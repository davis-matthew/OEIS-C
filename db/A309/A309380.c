/*
 * A309380  Number of unordered pairs of 5-colorings of an n-wheel that differ
 *          in the coloring of exactly one vertex.
 * https://oeis.org/A309380
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10*(2^(n-1) - 2*(-1)^n + (n-1)*(3^(n-2) - 3*(-1)^n))
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 37, u128 n <= 76,
 *            big any n
 * Helpers:   pow2, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    /* a(n) is defined for n >= 3 (offset = 3).  Below the offset we return 0. */
    if (n < 3) return 0;

    /* compute 2^(n-1) */
    seq_t pow2_n1 = pow2(n - 1);

    /* temp = 2^(n-1) - 2*(-1)^n */
    seq_t temp = pow2_n1;
    if (n & 1ULL)            /* n odd: -2*(-1)^n = +2 */
        temp += (seq_t)2;
    else                    /* n even: -2*(-1)^n = -2 */
        temp -= (seq_t)2;

    /* compute 3^(n-2) - 3*(-1)^n */
    seq_t pow3_n2 = pow3(n - 2);
    if (n & 1ULL)            /* n odd: -3*(-1)^n = +3 */
        pow3_n2 += (seq_t)3;
    else                    /* n even: -3*(-1)^n = -3 */
        pow3_n2 -= (seq_t)3;

    /* add (n-1)*(3^(n-2) - 3*(-1)^n) */
    temp += (seq_t)(n - 1) * pow3_n2;

    /* final multiplication by 10 */
    return temp * (seq_t)10;
}
