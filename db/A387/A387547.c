/*
 * A387547  Number of partitions of a 2n-set into even blocks of size > 2.
 * https://oeis.org/A387547
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=2..n} binomial(2*n-1, 2*k-1) * a(n-k), with
 *            initial values a(0) = 1, a(1) = 0
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;        /* cache of computed terms */
static idx_t cache_len = 0;        /* number of terms stored (size of cache) */

seq_t a(idx_t n)
{
    /* Ensure cache has space for a(0) and a(1) */
    if (cache_len == 0) {
        cache_len = 2;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_len);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)0;
    }

    /* Compute any missing terms up to n */
    if (n >= cache_len) {
        cache = (seq_t *)realloc(cache, sizeof(seq_t) * (n + 1));
        for (idx_t m = cache_len; m <= n; ++m) {
            if (m < 2) {
                /* m == 0 or 1 already set */
                continue;
            }
            seq_t sum = (seq_t)0;
            seq_t N = (seq_t)(2 * (seq_t)m - 1);   /* N = 2*m - 1 */
            /* start with t = 3 (k = 2) */
            seq_t t = (seq_t)3;
            /* r = C(N, 3) = N*(N-1)*(N-2)/6 */
            seq_t r = N * (N - 1);
            r *= (N - 2);
            r /= (seq_t)6;
            /* iterate k = 2 .. m */
            for (idx_t k = 2; k <= m; ++k) {
                /* add term for current k */
                sum += r * cache[m - k];

                /* prepare next r for t += 2 (next odd value) */
                if (k < m) {
                    /* C(N, t+2) = C(N, t) * (N - t) * (N - t - 1) / ((t + 1)*(t + 2)) */
                    seq_t next = r;
                    next *= (N - t);
                    next *= (N - (t + 1));
                    next /= (t + 1);
                    next /= (t + 2);
                    r = next;
                    t += (seq_t)2;
                }
            }
            cache[m] = sum;
        }
        cache_len = n + 1;
    }

    return cache[n];
}
