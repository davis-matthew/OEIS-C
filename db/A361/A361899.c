/*
 * A361899  a(n) = 3*(6858365065530*(2^45 - 1)*n + 153479820268467961)^2.
 * https://oeis.org/A361899
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*(6858365065530*(2^45 - 1)*n + 153479820268467961)^2
 * Range:     u8 none, u16 none, u32 none, u64 none, u128 n <= 0, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* 2^45 minus 1 */
    seq_t two45_minus1 = pow2(45) - (seq_t)1;

    /* Constant multiplier 6858365065530 */
    seq_t mult = (seq_t)6858365065530ULL;

    /* Coefficient c = mult * (2^45 - 1) */
    seq_t c = mult * two45_minus1;

    /* Offset d = 153479820268467961 */
    seq_t d = (seq_t)153479820268467961ULL;

    /* Intermediate t = c*n + d */
    seq_t t = c * (seq_t)n + d;

    /* Final result = 3 * t^2 */
    return (seq_t)3 * t * t;
}
