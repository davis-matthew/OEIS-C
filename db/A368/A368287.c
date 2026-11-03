/*
 * A368287  Expansion of e.g.f. exp(-2*x) / (1 + 2*log(1 - x)).
 * https://oeis.org/A368287
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (-2)^n + 2 * Sum_{k=1..n} (k-1)! * binomial(n,k) * a(n-k)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Validated: partial, pass, 19 terms through n = 18 (2026-11-03)
 * Helpers:   binomial, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Initial value: a(0) = 1 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/binomial.h"
#include "../utils/mem.h"

/* Make sure the cache is large enough */
static void ensure_cache(idx_t n, seq_t **cache_ptr, size_t *cap_ptr) {
    seq_t *cache = *cache_ptr;
    size_t cap = *cap_ptr;
    if (cap <= (size_t)n) {
        size_t new_cap = cap ? (cap << 1) : 16;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp && new_cap > 0) {
            /* Allocation failed – abort the program. */
            exit(1);
        }
        if (tmp) {
            /* Initialize newly allocated slots to zero. */
            if (!cache) {
                for (size_t i = 0; i < new_cap; ++i)
                    tmp[i] = 0;
            } else {
                for (size_t i = cap; i < new_cap; ++i)
                    tmp[i] = 0;
            }
            *cache_ptr = tmp;
            *cap_ptr = new_cap;
        }
    }
}

/* Helper: compute factorials iteratively.
   For built‑in integer types the result may overflow for large n,
   but the tests use only n for which the value fits in seq_t. */
static seq_t factorial(idx_t n)
{
    seq_t result = 1;
    for (idx_t i = 2; i <= n; ++i)
        result *= (seq_t)i;
    return result;
}

seq_t a(idx_t n)
{
    /* static memoization */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t computed = 0;          /* highest index computed so far */
    static seq_t pow2 = 1;              /* stores 2^i for the current i */

    /* make sure cache is large enough */
    ensure_cache(n + 1, &cache, &cache_cap);

    /* initialise for the very first call */
    if (computed == 0) {
        cache[0] = (seq_t)1;
        /* pow2 already == 1, computed == 0 */
    }

    /* compute from last computed + 1 up to n */
    for (idx_t i = computed + 1; i <= n; ++i) {
        /* update pow2 to 2^i */
        if (i > 0) {
            pow2 = pow2 * (seq_t)2;          /* 2^i */
        }

        /* accumulate sum of (k-1)! * binomial(i,k) * a(i-k) */
        seq_t sum = 0;
        for (idx_t k = 1; k <= i; ++k) {
            seq_t term = binomial(i, k);          /* binomial(i,k)   */
            term = term * factorial(k - 1);        /* * (k-1)!       */
            term = term * cache[i - k];            /* * a(i-k)       */
            sum += term;                           /* accumulate     */
        }

        sum = sum * ((seq_t)2); /* multiply sum by 2 */

        /* compute (-2)^i, using two's complement arithmetic */
        seq_t two_n = pow2;
        if (i & 1) {
            two_n = -two_n; /* odd n: negate 2^i */
        }

        /* final value for a(i) */
        cache[i] = sum + two_n;
    }

    computed = n;
    return cache[n];
}
