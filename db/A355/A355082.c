/*
 * A355082  G.f. A(x) satisfies A(x) = 1 + x * A(4 * x / (1 - x)) / (1 - x).
 * https://oeis.org/A355082
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, changed
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 1;
   a(n) = Sum_{k=0}^{n-1} 4^k * binomial(n-1,k) * a(k)   (n >= 1) */

#include <stdlib.h>

static seq_t *cache = NULL;          /* cache of computed values */
static idx_t cache_size = 0;         /* size of cache (number of initialized entries) */

seq_t a(idx_t n)
{
    /* Allocate cache large enough for indices 0..n */
    if (!cache) {
        cache = (seq_t *)malloc((n + 1) * sizeof(seq_t));
        cache[0] = (seq_t)1;               /* a(0) = 1 */
        cache_size = 1;
    } else if (n >= cache_size) {
        cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        /* cache_size remains the previous number of initialized entries */
    }

    /* Compute missing terms up to n */
    for (idx_t m = cache_size; m <= n; ++m) {
        seq_t sum   = (seq_t)0;
        seq_t binom = (seq_t)1;   /* C(m-1,0) */
        seq_t pow4  = (seq_t)1;   /* 4^0 */

        for (idx_t k = 0; k < m; ++k) {
            seq_t term = binom * pow4 * cache[k];
            sum += term;

            if (k < m - 1) {
                binom = (binom * (seq_t)(m - 1 - k)) / (seq_t)(k + 1);
                pow4  = pow4 * (seq_t)4;
            }
        }
        cache[m] = sum;
    }

    cache_size = n + 1;
    return cache[n];
}
