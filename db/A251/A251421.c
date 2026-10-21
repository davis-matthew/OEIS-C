/*
 * A251421  Number of length n+2 0..1 arrays with the sum of the maximum minus
 *          twice the median plus the minimum of adjacent triples multiplied
 *          by some arrangement of +-1 equal to zero.
 * https://oeis.org/A251421
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*(2^(n/2)+2^n) for even n; a(n) = 2*(2^n-2^((n-1)/2))
 *            for odd n
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* The sequence starts at n = 1.  For n = 0 we return 0. */
    if (n == 0) return 0;

    seq_t pow_n = pow2(n);

    if ((n & 1) == 0)          /* n even */
    {
        seq_t pow_half = pow2(n / 2);
        return 2 * (pow_half + pow_n);
    }
    else                       /* n odd */
    {
        seq_t pow_half = pow2((n - 1) / 2);  /* (n-1)/2 = (n-3)/2 + 1 */
        return 2 * (pow_n - pow_half);
    }
}
