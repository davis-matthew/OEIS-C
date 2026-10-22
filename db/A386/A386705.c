/*
 * A386705  a(n) = sum of the 2^(n-1) even positive integers having bit length
 *          2*n and in which, when written in binary, each run of 0's is of
 *          exactly the same length as the run of 1's immediately before it.
 * https://oeis.org/A386705
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 21, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A386705
    Empirical linear recurrence:
    a(n) = 12*a(n-1) - 36*a(n-2) + 32*a(n-3)
    with a(1)=2, a(2)=22, a(3)=192
*/

#ifndef A386705_H
#define A386705_H

#include <stddef.h>   /* for size_t, if needed */

/* Maximum n expected in the tests (≈10000). */
#define A386705_MAX_N 20000

/* Static cache for memoisation */
static seq_t a_cache[A386705_MAX_N + 1];
static idx_t a_cache_max = 0;

/* Compute a(n) for n >= 1 */
seq_t a(idx_t n)
{
    /* Guard against n outside the pre‑allocated range
       (the tests will not exceed A386705_MAX_N). */
    if (n == 0)
        return (seq_t)0;   /* undefined for n = 0, return 0 as a safe default */

    /* Ensure the cache covers the requested index */
    if (n > (idx_t)A386705_MAX_N) {
        /* If n exceeds the static array, grow it dynamically.
           Small dynamic memory helper: works for both C and C++. */
        static seq_t *dyn_cache = NULL;
        static idx_t dyn_cache_cap = 0;
        static idx_t dyn_cache_max = 0;

        if (n > dyn_cache_cap) {
            idx_t new_cap = (idx_t)(n * 2);          /* double to amortise allocations */
            seq_t *new_ptr;
#ifdef __cplusplus
            new_ptr = new seq_t[new_cap];
#else
            new_ptr = (seq_t*)malloc(new_cap * sizeof(seq_t));
#endif
            if (dyn_cache) {
#ifdef __cplusplus
                for (idx_t i = 0; i < dyn_cache_max; ++i) new_ptr[i] = dyn_cache[i];
                delete[] dyn_cache;
#else
                memcpy(new_ptr, dyn_cache, dyn_cache_max * sizeof(seq_t));
                free(dyn_cache);
#endif
            }
            dyn_cache = new_ptr;
            dyn_cache_cap = new_cap;
            dyn_cache_max = 0;
        }

        /* compute missing values */
        for (idx_t i = dyn_cache_max + 1; i <= n; ++i) {
            seq_t val;
            if (i == 1)          val = (seq_t)2;
            else if (i == 2)     val = (seq_t)22;
            else if (i == 3)     val = (seq_t)192;
            else /* i >= 4 */
                val = (seq_t)12 * dyn_cache[i-1] - (seq_t)36 * dyn_cache[i-2]
                      + (seq_t)32 * dyn_cache[i-3];
            dyn_cache[i] = val;
        }
        dyn_cache_max = n;
        return dyn_cache[n];
    }

    /* Use the static cache */
    if (n > a_cache_max) {
        for (idx_t i = a_cache_max + 1; i <= n; ++i) {
            seq_t val;
            if (i == 1)          val = (seq_t)2;
            else if (i == 2)     val = (seq_t)22;
            else if (i == 3)     val = (seq_t)192;
            else /* i >= 4 */
                val = (seq_t)12 * a_cache[i-1] - (seq_t)36 * a_cache[i-2]
                      + (seq_t)32 * a_cache[i-3];
            a_cache[i] = val;
        }
        a_cache_max = n;
    }
    return a_cache[n];
}

#endif /* A386705_H */
