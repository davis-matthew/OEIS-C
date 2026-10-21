/*
 * A288964  Number of key comparisons to sort all n! permutations of n
 *          elements by quicksort.
 * https://oeis.org/A288964
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * ( 2 * (n + 1) * H(n) - 4 * n ) (H(n) is the harmonic
 *            number 1 + 1/2 + ... + 1/n)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 19, u128 n <= 31,
 *            big any n
 * Helpers:   scalar_compute
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses two different strategies depending on the
 * size of seq_t.  For small integral types (<= 8 bytes) it computes the
 * value with 128‑bit intermediates, suitable for n up to 19 (the largest
 * value that fits in unsigned 64‑bit).  For larger seq_t types the
 * implementation builds the sequence incrementally using the recurrence:
 *
 *   a(0) = 0
 *   a(1) = 0
 *   a(2) = 2
 *   a(n) = ((2*n*n - 3*n - 1) * a(n-1) -
 *           ((n-1)*(n-1)*n) * a(n-2)) / (n-2)      for n >= 3
 *
 * The values are memoised in a static array so that calls are O(1) after
 * the first calculation of the required index.  No dynamic allocation is
 * used for the small‑type path.  This file is valid C11 and also compiles
 * as C++17.  No external libraries are required.  */

#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include "../utils/scalar_compute.h"

/* Static cache used only when seq_t is not a small builtin type. */
static seq_t *cache = NULL;
static size_t cache_cap = 0;

/* Ensure that cache has room for indices up to n (inclusive). */
static void ensure_cache(idx_t n)
{
    if ((idx_t)cache_cap > n) return;

    size_t new_cap = (size_t)n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    assert(tmp);
    cache = tmp;

    for (idx_t i = cache_cap; i <= n; ++i) {
        if (i == 0) {
            cache[0] = 0;
        } else if (i == 1) {
            cache[1] = 0;
        } else if (i == 2) {
            cache[2] = 2;
        } else {
            /* Recurrence:
             * a(n) = ((2*n*n - 3*n - 1) * a(n-1) -
             *        ((n-1)*(n-1)*n) * a(n-2)) / (n-2)
             */
            seq_t coef1 = (seq_t)((seq_t)(2 * (seq_t)i * (seq_t)i) -
                                  3 * (seq_t)i - 1);
            seq_t coef2 = (seq_t)(((seq_t)(i - 1) * (seq_t)(i - 1)) *
                                  (seq_t)i);
            seq_t prod1 = cache[i - 1] * coef1;
            seq_t prod2 = cache[i - 2] * coef2;
            seq_t num   = prod1 - prod2;
            cache[i] = num / (i - 2);
        }
    }

    cache_cap = new_cap;
}

/* Public interface. */
seq_t a(idx_t n)
{
    /* For small integral seq_t (including 8‑byte unsigned types)
     * use the exact 128‑bit calculation.  For larger seq_t (big integer
     * classes) use the lazy memoised recurrence. */
    if (sizeof(seq_t) <= sizeof(uint64_t)) {
        return scalar_compute(n);
    } else {
        ensure_cache(n);
        return cache[n];
    }
}
