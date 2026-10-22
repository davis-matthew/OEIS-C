/*
 * A379421  a(n) = n + floor(n*r/s) + floor(n*t/s), where r = 5^(1/4), s =
 *          5^(1/2), t = 5^(3/4).
 * https://oeis.org/A379421
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + floor(n/r) + floor(n*r), where r = 5^(1/4)
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   _floor_fourth_root
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/_floor_fourth_root.h"

seq_t a(idx_t n)
{
    seq_t k = (seq_t)n;            /* n in seq_t */
    /* n^4  */
    seq_t k2 = k * k;              /* k^2   */
    seq_t k4 = k2 * k2;            /* k^4   */

    /* floor(n*r)  = floor( (5*n^4)^(1/4) )  */
    seq_t m_r = _floor_fourth_root(5 * k4);

    /* floor(n/r)  = floor( (n^4 / 5)^(1/4) )  */
    seq_t m_ir = _floor_fourth_root(k4 / 5);

    return k + m_ir + m_r;
}
