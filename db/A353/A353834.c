/*
 * A353834  Nonprime numbers whose prime indices have all equal run-sums.
 * https://oeis.org/A353834
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) returns the n-th nonprime number whose prime indices
 *            (prime(p) divides the number) have all equal run-sums. The
 *            sequence begins 1,4,8,9,12,16,25,27,32,40,49,63,64,81,112,...
 *            The function builds a prime sieve on demand and caches
 *            previously computed terms. It is thread‑unsafe but satisfies
 *            th...
 * Range:     u8 n <= 21, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* Global state */
static uint32_t *spf = NULL;              /* smallest prime factor */
static uint32_t *prime_index = NULL;      /* 1‑based index of a prime, 0 if not prime */
static uint32_t *primes = NULL;           /* list of primes */
static size_t    primes_count = 0;        /* number of primes stored */
static idx_t     sieve_limit = 0;         /* current limit of the sieve */

static idx_t *cache = NULL;              /* cached values (idx_t) */
static size_t cache_len  = 0;             /* number of values cached */
static size_t cache_cap  = 0;             /* capacity of cache array */
static idx_t  next_candidate = 2;         /* next integer to test */

/* ------------------------------------------------------------------ */
/* Build a new sieve up to at least 'limit'.  Frees previous data.    */
static void build_sieve(idx_t limit)
{
    free(spf);      free(prime_index);   free(primes);
    spf = (uint32_t *)malloc((limit + 1) * sizeof(uint32_t));
    prime_index = (uint32_t *)malloc((limit + 1) * sizeof(uint32_t));

    size_t prim_cap = limit / 10 + 10;   /* rough prime count */
    primes = (uint32_t *)malloc(prim_cap * sizeof(uint32_t));

    memset(spf, 0, (limit + 1) * sizeof(uint32_t));
    memset(prime_index, 0, (limit + 1) * sizeof(uint32_t));

    size_t pcnt = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        if (spf[i] == 0) {                        /* i is prime */
            spf[i] = (uint32_t)i;
            if (pcnt == prim_cap) {                /* grow primes array */
                prim_cap *= 2;
                primes = (uint32_t *)realloc(primes,
                                             prim_cap * sizeof(uint32_t));
            }
            primes[pcnt++] = (uint32_t)i;
            prime_index[i] = (uint32_t)(pcnt);     /* 1‑based index */
        }
        for (size_t j = 0; j < pcnt &&
             (uint64_t)primes[j] * i <= limit; ++j) {
            idx_t prod = i * (idx_t)primes[j];
            spf[prod] = primes[j];
            if (primes[j] == spf[i]) break;
        }
    }
    primes_count = pcnt;
    sieve_limit = limit;
}

/* Ensure the sieve covers at least 'need'.  Expands by doubling.     */
static void ensure_sieve(idx_t need)
{
    if (need <= sieve_limit) return;
    idx_t new_limit = sieve_limit ? sieve_limit * 2 : 1000;
    if (new_limit < need) new_limit = need;
    build_sieve(new_limit);
}

/* ------------------------------------------------------------------ */
/* Check whether a number satisfies the run‑sum equality property.  */
static bool good_candidate(idx_t num)
{
    if (num == 1) return true;                /* 1 is explicitly accepted */

    if (spf[num] == num) return false;        /* prime numbers are excluded */

    idx_t t = num;
    bool first = true;
    idx_t common = 0;
    while (t > 1) {
        uint32_t p = spf[t];
        idx_t e = 0;
        while (t % p == 0) {
            t /= p;
            ++e;
        }
        idx_t idx = prime_index[p];           /* 1‑based prime index */
        idx_t prod = idx * e;
        if (first) {
            common = prod;
            first = false;
        } else if (prod != common) {
            return false;
        }
    }
    return true;
}

/* ------------------------------------------------------------------ */
/* Ensure that cache contains at least 'n' terms.                     */
static void ensure_cache(idx_t n)
{
    if (cache_len >= n) return;

    /* Initialise cache if empty */
    if (cache_len == 0) {
        cache_cap = 64;
        cache = (idx_t *)malloc(cache_cap * sizeof(idx_t));
        cache[0] = 1;                /* a(1) = 1 */
        cache_len = 1;
        next_candidate = 2;
        build_sieve(1000);           /* start with moderate sieve */
    }

    /* Grow cache until we have 'n' terms */
    while (cache_len < n) {
        if (next_candidate > sieve_limit) {
            /* Grow sieve when needed */
            ensure_sieve(next_candidate * 2);
        }
        if (good_candidate(next_candidate)) {
            if (cache_len == cache_cap) {
                cache_cap *= 2;
                cache = (idx_t *)realloc(cache, cache_cap * sizeof(idx_t));
            }
            cache[cache_len++] = next_candidate;
        }
        ++next_candidate;
    }
}

/* ------------------------------------------------------------------ */
/* Required function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                    /* undefined for offset < 1 */
    ensure_cache(n);
    return (seq_t)cache[n - 1];              /* a(1) stored at index 0 */
}
