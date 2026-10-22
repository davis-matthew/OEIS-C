/*
 * A357366  Expansion of Product_{k>=0} 1 / (1 - x^(2^k) - x^(2^(k+1)))^(2^k).
 * https://oeis.org/A357366
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=1. For n>=2: a(n) = a(n-1) + a(n-2) + (n even ?
 *            Σ_{i=0}^{n/2} a(i) * a(n/2-i) : 0)
 * Range:     u8 n <= 8, u16 n <= 19, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This recurrence follows from (1 – x – x²)A(x) = A(x²)². */
#include <stdlib.h>

static seq_t *cache = NULL;          /* cached terms a(0)…a(max_cached-1) */
static idx_t max_cached = 0;         /* number of terms currently stored */

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Grow cache if needed */
    if (n >= max_cached) {
        /* Allocate until we have at least n+1 slots */
        size_t newsize = (size_t)(n + 1);
        cache = (seq_t *)realloc(cache, newsize * sizeof(seq_t));
        if (!cache) abort();           /* allocation failed */

        /* Initialize the base case if this is the first call */
        if (max_cached == 0) {
            cache[0] = (seq_t)1;
            max_cached = 1;
        }

        /* Compute missing terms up to n */
        for (idx_t i = max_cached; i <= n; ++i) {
            seq_t val = 0;
            if (i >= 1) val += cache[i - 1];
            if (i >= 2) val += cache[i - 2];

            /* Add convolution term when i is even */
            if ((i & 1u) == 0) {
                idx_t m = i >> 1;          /* i/2 */
                seq_t sum = 0;
                for (idx_t t = 0; t <= m; ++t) {
                    sum += cache[t] * cache[m - t];
                }
                val += sum;
            }
            cache[i] = val;
        }
        max_cached = n + 1;
    }

    return cache[n];
}
