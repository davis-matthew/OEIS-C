/*
 * A290588  Number of irredundant sets in the n-helm graph.
 * https://oeis.org/A290588
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1) - 10*a(n-2) + 4*a(n-3) - 5*a(n-4) + 6*a(n-5),
 *            with a(1)=5, a(2)=13, a(3)=35, a(4)=113, a(5)=355
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   initial_value
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: a(n) is defined for n ≥ 1. */

#include <stdlib.h>   /* for malloc, realloc */
#include <stddef.h>   /* for size_t */
#include "../utils/initial_value.h"

static seq_t *cache = NULL;    /* pointer to cached values */
static idx_t  size_cached = 0; /* number of already computed terms */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n=0, return 0 */

    /* Expand cache if needed */
    if (n > size_cached) {
        size_t new_size = (size_t)n;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) {
            /* Allocation failed: return 0 (or could abort) */
            return (seq_t)0;
        }
        cache = tmp;

        /* Compute terms from current size to n */
        for (idx_t i = size_cached + 1; i <= n; ++i) {
            seq_t val;
            if (i <= 5) {
                val = initial_value(i);
            } else {
                /* Use recurrence: a(i) = 6*a(i-1) - 10*a(i-2) + 4*a(i-3) - 5*a(i-4) + 6*a(i-5) */
                seq_t a1 = cache[(size_t)(i - 2)]; /* a(i-1)   */
                seq_t a2 = cache[(size_t)(i - 3)]; /* a(i-2)   */
                seq_t a3 = cache[(size_t)(i - 4)]; /* a(i-3)   */
                seq_t a4 = cache[(size_t)(i - 5)]; /* a(i-4)   */
                seq_t a5 = cache[(size_t)(i - 6)]; /* a(i-5)   */

                val = (seq_t)6 * a1;         /* +6*a(i-1) */
                val -= (seq_t)10 * a2;        /* -10*a(i-2) */
                val += (seq_t)4  * a3;        /* +4*a(i-3) */
                val -= (seq_t)5  * a4;        /* -5*a(i-4) */
                val += (seq_t)6  * a5;        /* +6*a(i-5) */
            }
            cache[(size_t)(i - 1)] = val;
        }
        size_cached = n;
    }

    return cache[(size_t)(n - 1)];
}
