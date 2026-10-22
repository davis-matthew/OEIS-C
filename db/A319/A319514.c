/*
 * A319514  The shell enumeration of N X N where N = {0, 1, 2, ...}, also
 *          called boustrophedonic Rosenberg-Strong function. Terms are
 *          interleaved x and y coordinates.
 * https://oeis.org/A319514
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(2k) = x_k, a(2k+1) = y_k where (x_k, y_k) is the k‑th point in
 *            the boustrophedonic Rosenberg–Strong enumeration of ℕ×ℕ. The
 *            enumeration proceeds by layers: for m = 0,1,2,… add the 2m+1
 *            points with max(x,y)=m in the order depending on the parity of m
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>

seq_t a(idx_t n)
{
    /* integer division gives k = n/2 */
    idx_t k = n >> 1;
    /* floor sqrt of k (k ≤ 2^63, safe in double) */
    idx_t m = (idx_t) sqrt((double)k);
    idx_t d = k - m * m;   /* offset within current layer, 0 … 2m */

    seq_t x, y;

    if ((m & 1) == 0) {          /* even layer m */
        if (d <= m) {
            x = (seq_t)m;
            y = (seq_t)d;
        } else {
            x = (seq_t)(2 * m - d);
            y = (seq_t)m;
        }
    } else {                     /* odd layer m */
        if (d <= m) {
            x = (seq_t)d;
            y = (seq_t)m;
        } else {
            x = (seq_t)m;
            y = (seq_t)(2 * m - d);
        }
    }

    return (n & 1) ? y : x;
}
