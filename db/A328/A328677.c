/*
 * A328677  Numbers whose distinct prime indices are relatively prime and
 *          pairwise indivisible.
 * https://oeis.org/A328677
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 46, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   _gcd_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function finds the n‑th positive integer whose set of
   distinct prime indices is pairwise coprime and pairwise indivisible.
   The algorithm tests successive integers, uses a pre‑computed
   smallest‑prime‑factor table, and checks the prime indices of each
   factor.  It caches the found values so that calls in any order are
   efficient.  */

/* Required standard headers */
#include <stdlib.h>   /* malloc, realloc */
#include <stdbool.h>  /* bool type */
#include <stdint.h>   /* uint32_t */
#include "../utils/_gcd_idx.h"

/* Prime sieve parameters */
#define SIEVE_LIMIT 5000000U  /* enough for n up to about 10 000 */

/* Global tables for the sieve */
static uint32_t *spf          = NULL; /* smallest prime factor for each number */
static uint32_t *prime_index   = NULL; /* index of prime (0 if composite) */
static uint32_t *primes        = NULL; /* list of primes */
static size_t    prime_cnt     = 0;    /* number of primes found */

/* Global cache for the sequence */
static seq_t *cache   = NULL; /* cache of a(k) values */
static size_t cache_len  = 0;  /* number of values stored */
static size_t cache_cap  = 0;  /* allocated capacity of cache */

/* Current integer being examined */
static idx_t current_n = 2;

/* ----------------------------------------------------------------------
   Utility: compute gcd of two idx_t values.  Used only internally.
   ---------------------------------------------------------------------- */

/* ----------------------------------------------------------------------
   Initialize the sieve tables on first use.  The arrays are large
   enough to hold all numbers examined for n up to about 10 000.
   ---------------------------------------------------------------------- */
static void init_sieve(void)
{
    if (spf != NULL) return;   /* already initialized */

    /* Allocate spf and prime_index arrays */
    spf = (uint32_t *)malloc((SIEVE_LIMIT + 1) * sizeof(uint32_t));
    prime_index = (uint32_t *)malloc((SIEVE_LIMIT + 1) * sizeof(uint32_t));
    if (spf == NULL || prime_index == NULL) exit(1);

    /* Set default spf to the number itself */
    for (idx_t i = 0; i <= SIEVE_LIMIT; ++i) spf[i] = (uint32_t)i;

    /* Sieve for smallest prime factor */
    for (idx_t i = 2; i * i <= SIEVE_LIMIT; ++i) {
        if (spf[i] == (uint32_t)i) {
            for (idx_t j = i * i; j <= SIEVE_LIMIT; j += i) {
                if (spf[j] == (uint32_t)j) spf[j] = (uint32_t)i;
            }
        }
    }

    /* Build list of primes and prime_index table */
    primes = (uint32_t *)malloc((SIEVE_LIMIT / 10 + 100) * sizeof(uint32_t));
    if (primes == NULL) exit(1);
    for (idx_t i = 2; i <= SIEVE_LIMIT; ++i) {
        if (spf[i] == (uint32_t)i) {        /* i is prime */
            primes[prime_cnt++] = (uint32_t)i;
            prime_index[i] = (uint32_t)prime_cnt;  /* indices start at 1 */
        }
    }
}

/* ----------------------------------------------------------------------
   Determine whether integer m satisfies the required property.
   The function assumes m >= 2 and m <= SIEVE_LIMIT.
   ---------------------------------------------------------------------- */
static inline bool is_good_m(idx_t m)
{
    /* Collect distinct prime indices of m */
    uint32_t indices[32];
    int len = 0;

    while (m > 1) {
        uint32_t p = spf[m];
        while (m % p == 0) m /= p;          /* remove all copies of p */
        uint32_t idx = prime_index[p];      /* prime index of p */

        /* Skip if this index already recorded */
        bool seen = false;
        for (int i = 0; i < len; ++i) {
            if (indices[i] == idx) {
                seen = true;
                break;
            }
        }
        if (!seen) indices[len++] = idx;
    }

    if (len == 1) {
        /* Only powers of 2 are allowed (prime index 1) */
        return indices[0] == 1;
    }

    /* Reject if 1 appears together with any other index */
    for (int i = 0; i < len; ++i) {
        if (indices[i] == 1) return false;
    }

    /* Pairwise coprime check */
    for (int i = 0; i < len; ++i) {
        for (int j = i + 1; j < len; ++j) {
            if (_gcd_idx(indices[i], indices[j]) > 1) return false;
        }
    }
    return true;
}

/* ----------------------------------------------------------------------
   Generate terms of the sequence until we have at least "target"
   terms stored in the global cache.
   ---------------------------------------------------------------------- */
static void generate_until(size_t target)
{
    if (cache_len >= target) return;

    if (cache == NULL) {
        cache_cap = (target > 64) ? target : 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) exit(1);
    }

    init_sieve();

    for (; cache_len < target; ++current_n) {
        if (current_n > SIEVE_LIMIT) {
            /* In the unlikely event we exceed the sieve, terminate. */
            break;
        }
        if (is_good_m(current_n)) {
            if (cache_len == cache_cap) {
                cache_cap <<= 1;
                seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                if (tmp == NULL) exit(1);
                cache = tmp;
            }
            cache[cache_len++] = (seq_t)current_n;
        }
    }
}

/* ----------------------------------------------------------------------
   Public function requested by the problem statement.
   ---------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    generate_until((size_t)n);
    return cache[n - 1];
}
