/*
 * A257313  a(1) = 10; a(2) = 11; for n > 2, a(n) is the smallest number of
 *          the form prime + 8 not already used which shares a factor with
 *          a(n-1).
 * https://oeis.org/A257313
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=10, a(2)=11. For n>2, a(n) is the smallest number of the
 *            form prime+8 not already used which shares a non‑trivial factor
 *            with a(n-1). Implemented with a dynamic sieve and sequential
 *            search
 * Range:     u8 n <= 20, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/gcd.h"

/*--------------------------------------------------------------------------*/
/* Sequence cache and state                                               */
/*--------------------------------------------------------------------------*/
static bool      initialized = false;
static seq_t    *seq_cache   = NULL;   /* computed sequence terms          */
static size_t    seq_cap     = 0;       /* allocated cache capacity         */
static size_t    seq_len     = 0;       /* number of terms currently stored */

static uint64_t *primes      = NULL;   /* list of primes up to primes_limit */
static size_t    primes_cnt  = 0;       /* number of primes collected        */
static idx_t     primes_limit = 0;      /* largest prime generated so far    */

static bool     *used_prime  = NULL;   /* used_prime[p] == true if prime p
                                         has been used in the sequence    */

/*--------------------------------------------------------------------------*/
/* Ensure that primes up to 'limit' are generated.                          */
/*--------------------------------------------------------------------------*/
static void ensure_primes_limit(idx_t limit)
{
    if ((idx_t)primes_limit >= limit)
        return;                     /* already sufficient */

    /* ---------------------------------------------------------------------*/
    /* Build a sieve of Eratosthenes up to 'limit' (inclusive).             */
    /* ---------------------------------------------------------------------*/
    size_t sz = (size_t)limit + 1;
    bool *is_comp = (bool *)calloc(sz, sizeof(bool));
    if (!is_comp)
        abort();              /* memory allocation failed */

    for (idx_t i = 2; i * i <= limit; ++i) {
        if (!is_comp[(size_t)i]) {
            for (idx_t j = i * i; j <= limit; j += i)
                is_comp[(size_t)j] = true;
        }
    }

    /* Count primes. */
    size_t new_primes_cnt = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (!is_comp[(size_t)i])
            ++new_primes_cnt;

    /* Allocate and store primes. */
    uint64_t *new_primes = (uint64_t *)malloc(new_primes_cnt * sizeof(uint64_t));
    if (!new_primes)
        abort();

    size_t idx = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (!is_comp[(size_t)i])
            new_primes[idx++] = (uint64_t)i;
    free(is_comp);

    /* ---------------------------------------------------------------------*/
    /* Resize and preserve 'used_prime' flags.                               */
    /* ---------------------------------------------------------------------*/
    bool *new_used_prime = (bool *)calloc((size_t)limit + 1, sizeof(bool));
    if (!new_used_prime)
        abort();

    if (used_prime) {
        size_t existing = (size_t)primes_limit + 1;
        if (existing > (size_t)limit + 1)
            existing = (size_t)limit + 1;
        memcpy(new_used_prime, used_prime,
               existing * sizeof(bool));
        free(used_prime);
    }

    /* ---------------------------------------------------------------------*/
    /* Replace the old data with the new ones.                               */
    /* ---------------------------------------------------------------------*/
    free(primes);
    primes      = new_primes;
    primes_cnt  = new_primes_cnt;
    primes_limit = limit;
    used_prime  = new_used_prime;
}

/*--------------------------------------------------------------------------*/
/* Find the next sequence term after 'prev'.                                */
/*--------------------------------------------------------------------------*/
static seq_t find_next_candidate(seq_t prev)
{
    while (true) {
        for (size_t pi = 0; pi < primes_cnt; ++pi) {
            uint64_t p = primes[pi];
            if (used_prime[(size_t)p])
                continue;                      /* already used */

            seq_t cand = (seq_t)(p + 8ULL);     /* prime + 8 */
            if (gcd(prev, cand) > (seq_t)1) {
                used_prime[(size_t)p] = true; /* mark as used */
                return cand;
            }
        }
        /* No candidate found with current primes → enlarge the sieve. */
        idx_t new_limit = (primes_limit == 0) ? 1000U : (primes_limit << 1);
        if (new_limit <= primes_limit)
            new_limit = primes_limit + 1000U;
        ensure_primes_limit(new_limit);
    }
}

/*--------------------------------------------------------------------------*/
/* Public sequence function.                                                */
/*--------------------------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;       /* undefined for n<1 */

    /* First‑time initialization. */
    if (!initialized) {
        initialized = true;
        seq_cap = 32;
        seq_cache = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        if (!seq_cache)
            abort();

        seq_len = 0;
        ensure_primes_limit(200000);          /* initial sieve size */

        /* Mark primes that generate the first two terms. */
        used_prime[(size_t)2] = true;        /* 2 + 8 = 10  */
        used_prime[(size_t)3] = true;        /* 3 + 8 = 11  */
    }

    /* Compute terms up to index n (1‑based). */
    while (seq_len < n) {
        seq_t next;

        if (seq_len == 0)
            next = (seq_t)10;                      /* a(1) */
        else if (seq_len == 1)
            next = (seq_t)11;                      /* a(2) */
        else
            next = find_next_candidate(seq_cache[seq_len - 1]);

        /* Ensure cache capacity. */
        if (seq_len >= seq_cap) {
            seq_cap <<= 1;
            seq_cache = (seq_t *)realloc(seq_cache,
                                         seq_cap * sizeof(seq_t));
            if (!seq_cache)
                abort();
        }

        seq_cache[seq_len++] = next;
    }

    return seq_cache[n - 1];
}
