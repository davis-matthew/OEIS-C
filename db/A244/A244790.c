/*
 * A244790  Number of length 2 0..n arrays with each partial sum starting from
 *          the beginning no more than one standard deviation from its mean.
 * https://oeis.org/A244790
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 209, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   _max_w
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A244790
   Number of arrays [x,y] with 0 <= x,y <= n such that for each
   k = 1,2 the partial sum S_k = x + ... + last element of the
   first k entries satisfies
          |S_k - k*n/2| <= sqrt(k*n*(n+2)/12).
   Equivalently:
          3*(2*x - n)^2 <= n*(n+2)          (k = 1)
          6*(x + y - n)^2 <= n*(n+2)         (k = 2)
*/
#include <stdint.h>
#include "../utils/_max_w.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    seq_t N     = (seq_t)n * ((seq_t)n + 2);   /* N = n*(n+2)   */
    seq_t max_w = _max_w(n);                  /* w_max for condition |x+y-n| <= sqrt(N/6) */

    seq_t total = 0;
    for (idx_t x = 0; x <= n; ++x) {
        int64_t t   = (int64_t)2 * (int64_t)x - (int64_t)n;   /* t = 2x - n  */
        seq_t  t2   = (seq_t)t * (seq_t)t;                    /* t^2        */
        if (3 * t2 <= N) {                                   /* condition for x */
            int64_t nx = (int64_t)n - (int64_t)x;             /* n - x      */
            int64_t low  = nx - (int64_t)max_w;
            if (low < 0) low = 0;
            int64_t high = nx + (int64_t)max_w;
            if (high > (int64_t)n) high = n;
            if (high >= low)
                total += (seq_t)(high - low + 1);
        }
    }
    return total;
}
