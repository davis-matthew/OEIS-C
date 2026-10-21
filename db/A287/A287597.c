/*
 * A287597  a(0) = 0, a(1) = 1; a(2*n) = n - a(a(n)), a(2*n+1) = a(a(n)) +
 *          a(a(n+1)).
 * https://oeis.org/A287597
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(0) = 0, a(1) = 1; a(2*n) = n - a(a(n)),
 *          a(2*n+1) = a(a(n)) + a(a(n+1))
 */

#include <stdlib.h>
#include <stdbool.h>
#ifdef __cplusplus
#include <vector>
#endif

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 1;

#ifdef __cplusplus
    /* C++ implementation: use std::vector for automatic construction */
    static std::vector<seq_t> cache;
    if (cache.size() <= n) cache.resize(n + 1);
    cache[0] = 0;
    cache[1] = 1;

    for (idx_t i = 2; i <= n; ++i) {
        if ((i & 1) == 0) {                     /* even: i = 2*t */
            idx_t   t  = i >> 1;
            seq_t   v1 = cache[t];
            seq_t   v2 = cache[(idx_t)v1];
            cache[i] = (seq_t)t - v2;
        } else {                                 /* odd: i = 2*t+1 */
            idx_t   t  = i >> 1;
            seq_t   a_t  = cache[t];
            seq_t   a_t1 = cache[t + 1];
            seq_t   v1   = cache[(idx_t)a_t];
            seq_t   v2   = cache[(idx_t)a_t1];
            cache[i] = v1 + v2;
        }
    }
    return cache[n];
#else
    /* C implementation: allocate a raw array (seq_t must be a POD type) */
    seq_t *buf = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!buf) abort();
    buf[0] = 0;
    if (n >= 1) buf[1] = 1;

    for (idx_t i = 2; i <= n; ++i) {
        if ((i & 1) == 0) {                     /* even: i = 2*t */
            idx_t   t  = i >> 1;
            seq_t   v1 = buf[t];
            seq_t   v2 = buf[(idx_t)v1];
            buf[i] = (seq_t)t - v2;
        } else {                                 /* odd: i = 2*t+1 */
            idx_t   t  = i >> 1;
            seq_t   a_t  = buf[t];
            seq_t   a_t1 = buf[t + 1];
            seq_t   v1   = buf[(idx_t)a_t];
            seq_t   v2   = buf[(idx_t)a_t1];
            buf[i] = v1 + v2;
        }
    }

    seq_t result = buf[n];
    free(buf);
    return result;
#endif
}
