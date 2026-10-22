/*
 * A397672  Value of the Colless index for the rooted binary echelon tree with
 *          n leaves.
 * https://oeis.org/A397672
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*m - n + a(n-m) for n > 2^k, a(2^k)=0; where m is the
 *            largest power of two <= n (k = floor(log2 n))
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t res = 0;
    while (n > 1)
    {
        /* stop when reaching a power of two, since a(2^k)=0 */
        if ((n & (n - 1)) == 0)
            break;

        /* find the largest power of two <= n */
        idx_t m = 1;
        while (m <= n)
            m <<= 1;
        m >>= 1;

        /* accumulate contribution */
        res += (seq_t)2 * (seq_t)m - (seq_t)n;
        n -= m;
    }
    return res;
}
