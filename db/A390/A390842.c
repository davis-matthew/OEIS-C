/*
 * A390842  3-valuation of Product_{k=1..n} (prime(k) + 1) / (prime(k+1) -
 *          prime(k)).
 * https://oeis.org/A390842
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=1..n} (v₃(prime(k)+1) – v₃(prime(k+1)‑prime(k)))
 * Range:     u8 n <= 703, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime, v3_uint32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where v₃(m) is the exponent of 3 in the factorisation of m. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/get_prime.h"
#include "../utils/v3_uint32.h"

/* The build supplies these types and helper functions */
extern idx_t _floor_isqrt_u64(idx_t n);

/* Static storage for memoization */
static seq_t *a_cache = NULL;   /* array of computed a(n) values */
static size_t a_cap   = 0;      /* current capacity of a_cache */
static idx_t  max_cached = (idx_t)-1;   /* largest n for which a_cache[n] is valid */

/* Ensure the cache can hold indices up to 'needed' (inclusive). */
static void grow_cache(size_t needed)
{
    if (needed <= a_cap) return;
    size_t new_cap = a_cap ? a_cap * 2 : 4;
    while (new_cap < needed) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
    if (!tmp) exit(1);            /* out of memory */
    a_cache = tmp;
    a_cap   = new_cap;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0U) return 0U;         /* a(0) = 0 by definition */

    /* Initialise cache on first call */
    if (a_cache == NULL) {
        grow_cache(2);
        a_cache[0] = 0U;            /* a(0) */
        max_cached  = 0U;
    }

    if (n > max_cached) {
        /* Extend cache to hold value for n */
        grow_cache((size_t)n + 1);

        /* Compute successive values from max_cached+1 up to n */
        for (idx_t k = max_cached + 1U; k <= n; ++k) {
            uint32_t pk  = get_prime(k);       /* k-th prime, 1‑based */
            uint32_t pk1 = get_prime(k + 1);   /* (k+1)-th prime */

            int vnum = v3_uint32(pk + 1U);
            int vden = v3_uint32(pk1 - pk);

            int64_t val = (int64_t)a_cache[(size_t)k - 1U] + (int64_t)vnum - (int64_t)vden;
            a_cache[(size_t)k] = (seq_t)val;
        }
        max_cached = n;
    }

    return a_cache[(size_t)n];
}
