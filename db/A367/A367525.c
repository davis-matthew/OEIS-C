/*
 * A367525  The number of ways of tiling the n X n grid up to the symmetries
 *          of the square by a tile that is not fixed under any of the
 *          symmetries of the square.
 * https://oeis.org/A367525
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(2m-1) = 4096^(m^2 - m); a(2m) = 8^(m^2 - 1)*(512^m^2 + 3*8^m^2
 *            + 2)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 6,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1, but guard for safety */

    if (n & 1)                     /* odd index: a(2m-1) */
    {
        /* m = (n+1)/2  */
        idx_t m  = (n + 1) / 2;
        idx_t mm = m * m;
        /* exponent = 12*(m^2 - m) because 4096 = 2^12 */
        idx_t exp = 12 * (mm - m);
        return pow2(exp);
    }
    else                            /* even index: a(2m) */
    {
        idx_t m  = n / 2;
        idx_t mm = m * m;

        /* part1 = 8^(m^2 - 1) = 2^(3*(m^2 - 1)) */
        idx_t exp1 = 3 * (mm - 1);
        seq_t part1 = pow2(exp1);

        /* t1 = 512^(m^2) = 2^(9*m^2) */
        seq_t t1 = pow2(9 * mm);

        /* t2 = 8^(m^2)   = 2^(3*m^2)  */
        seq_t t2 = pow2(3 * mm);

        /* term2 = t1 + 3*t2 + 2 */
        seq_t three = 3;
        seq_t two   = 2;
        seq_t term2 = t1 + (t2 * three) + two;

        return part1 * term2;
    }
}
