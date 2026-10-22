/*
 * A342198  a(0) = 1; a(n) = Sum_{k=1..n} binomial(n,k)^4 * a(k-1).
 * https://oeis.org/A342198
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} binomial(n,k)^4 * a(k-1)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 1, a(n) = Σ_{k=1..n} C(n,k)^4 · a(k-1)       */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    /* Static cache to store computed values in any order of calls */
    static seq_t *cache = NULL;   /* pointer to array of a-values   */
    static idx_t size = 0;        /* number of initialized elements */
    static idx_t cap  = 0;        /* allocated capacity (>=size)   */

    /* Initialise cache with a(0) if it has not been done yet */
    if (cap == 0) {
        cap = 1;
        cache = seq_alloc(cap);
        cache[0] = 1;            /* a(0) = 1                         */
        size = 1;
    }

    /* Expand capacity if needed (doubling strategy) */
    if ((idx_t)(n + 1) > cap) {
        idx_t new_cap = cap;
        if (new_cap == 0) new_cap = 1;
        while (new_cap <= n) new_cap <<= 1;
        seq_t *tmp = seq_realloc(cache, cap, new_cap);
        cache = tmp;
        cap = new_cap;
    }

    /* Compute missing terms up to n */
    for (idx_t i = size; i <= n; ++i) {
        seq_t sum   = 0;
        seq_t binom = 0;          /* will hold C(i,k) during the loop */

        for (idx_t k = 1; k <= i; ++k) {
            /* Compute C(i,k) multiplicatively */
            if (k == 1)
                binom = (seq_t)i;
            else
                binom = binom * (seq_t)(i - k + 1) / (seq_t)k;

            /* binom^4 */
            seq_t sq  = binom * binom;      /* binom^2          */
            seq_t q4  = sq * sq;            /* binom^4          */

            /* Add contribution q4 * a(k-1) */
            seq_t term = q4 * cache[k - 1];
            sum += term;
        }
        cache[i] = sum;
    }
    size = n + 1;

    return cache[n];
}
