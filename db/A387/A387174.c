/*
 * A387174  a(n) is a prime that is the average of two distinct primes p and
 *          q, neither of which has been used before as such. However, the
 *          average can be any prime, including previous p's or q's.
 * https://oeis.org/A387174
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS sequence A387174 by a simple sieve and incremental search.
   The function a(n) returns the nth term (a(1)=5).  The algorithm
   maintains a list of primes up to a fixed upper bound, a table of
   unused primes, and a cache of already computed terms.  It is fast
   enough for n up to about 10 000 and works with arbitrary-precision
   seq_t because all intermediate arithmetic fits in 64 bit. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* maximum prime we will sieve: large enough for n≈10000 */
#define PRIME_LIMIT (10000000)

/* global state */
static bool *is_prime = NULL;          /* array[0..PRIME_LIMIT] */
static uint32_t *primes = NULL;        /* primes up to PRIME_LIMIT */
static size_t primes_count = 0;        /* number of primes stored */
static int *prime_index = NULL;        /* index of prime in primes[] or -1 */
static bool *used_primes = NULL;       /* which primes have been used */

static int *temp_indices = NULL;       /* temporary storage of indices */
static size_t temp_cap = 0;

static seq_t *cache = NULL;            /* cache of sequence values */
static size_t cache_len = 0;
static size_t cache_cap = 0;

static uint32_t candidate = 2;         /* next prime candidate */

/* ------------------------------------------------------------------ */
/* utility functions -------------------------------------------------- */
/* ------------------------------------------------------------------ */
static void build_sieve(void)
{
    /* allocate and initialise is_prime */
    is_prime = (bool *)calloc(PRIME_LIMIT + 1, sizeof(bool));
    if (!is_prime) exit(1);
    for (uint32_t i = 2; i <= PRIME_LIMIT; ++i) is_prime[i] = true;

    /* sieve */
    for (uint32_t i = 2; (uint64_t)i * i <= PRIME_LIMIT; ++i)
        if (is_prime[i])
            for (uint32_t j = i * i; j <= PRIME_LIMIT; j += i)
                is_prime[j] = false;

    /* allocate space for primes (≈ 10% of limit is enough) */
    primes = (uint32_t *)malloc((PRIME_LIMIT / 10 + 100) * sizeof(uint32_t));
    if (!primes) exit(1);

    primes_count = 0;
    for (uint32_t i = 2; i <= PRIME_LIMIT; ++i)
        if (is_prime[i]) primes[primes_count++] = i;

    /* prime_index for fast lookup of a prime's index in primes[] */
    prime_index = (int *)calloc(PRIME_LIMIT + 1, sizeof(int));
    if (!prime_index) exit(1);
    for (size_t i = 0; i < primes_count; ++i)
        prime_index[primes[i]] = (int)i;

    /* mark all primes as unused initially */
    used_primes = (bool *)calloc(primes_count, sizeof(bool));
    if (!used_primes) exit(1);

    /* temporary vector for indices of a valid representation */
    temp_cap = primes_count * 2 + 10;
    temp_indices = (int *)malloc(temp_cap * sizeof(int));
    if (!temp_indices) exit(1);
}

/* ensure cache can hold at least needed elements */
static void ensure_cache_cap(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 4;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *new_mem = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_mem) exit(1);
    cache = new_mem;
    cache_cap = new_cap;
}

/* ------------------------------------------------------------------ */
/* compute next term of the sequence -------------------------------- */
/* ------------------------------------------------------------------ */
static seq_t compute_next_term(void)
{
    for (;;)
    {
        /* advance to next prime if needed */
        while (candidate <= PRIME_LIMIT && !is_prime[candidate]) ++candidate;
        if (candidate > PRIME_LIMIT) exit(1); /* safety guard */

        /* look for all representations of candidate as (p+q)/2 */
        size_t temp_len = 0;
        for (size_t i = 0; i < primes_count && primes[i] <= candidate; ++i)
        {
            if (used_primes[i]) continue;
            uint32_t p = primes[i];
            uint32_t q = 2 * candidate - p;
            if (q <= p) continue;               /* distinct and ordered */
            if (q > PRIME_LIMIT) continue;
            if (!is_prime[q]) continue;
            int idx_q = prime_index[q];
            if (idx_q < 0 || used_primes[idx_q]) continue;

            /* store indices for later marking */
            if (temp_len + 2 > temp_cap)
            {
                temp_cap *= 2;
                int *new_mem = (int *)realloc(temp_indices, temp_cap * sizeof(int));
                if (!new_mem) exit(1);
                temp_indices = new_mem;
            }
            temp_indices[temp_len++] = (int)i;
            temp_indices[temp_len++] = idx_q;
        }

        if (temp_len == 0)
        {
            /* no valid pair: skip this candidate */
            ++candidate;
            continue;
        }

        /* mark all primes used in this representation */
        for (size_t k = 0; k < temp_len; ++k)
            used_primes[(size_t)temp_indices[k]] = true;

        /* the prime itself becomes the next term */
        seq_t result = (seq_t)candidate;
        ++candidate;
        return result;
    }
}

/* ------------------------------------------------------------------ */
/* public function --------------------------------------------------- */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    static bool initialized = false;
    if (!initialized)
    {
        build_sieve();
        cache = (seq_t *)malloc(0);   /* empty initially */
        cache_cap = 0;
        cache_len = 0;
        initialized = true;
    }

    if (n == 0) return (seq_t)0;   /* out of bounds, offset=1 */

    size_t target = (size_t)n;     /* we need terms up to a(n) inclusive */
    ensure_cache_cap(target);

    while (cache_len < target)
    {
        seq_t term = compute_next_term();
        cache[cache_len++] = term;
    }

    return cache[cache_len - 1];
}
