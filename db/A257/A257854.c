/*
 * A257854  a(n) = 2*n^4 - floor(2^(1/4)*n)^4.
 * https://oeis.org/A257854
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n^4 - floor(2^(1/4)*n)^4
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   floor_sqrt, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"
#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    seq_t n_seq = (seq_t)n;
    seq_t n4 = pow_seq(n_seq, 4);          /* n^4 */
    seq_t two_n4 = n4 + n4;                /* 2*n^4 */
    seq_t sqrt1 = floor_sqrt(two_n4);     /* floor(sqrt(2*n^4)) */
    seq_t sqrt2 = floor_sqrt(sqrt1);      /* floor(sqrt(floor(sqrt(2*n^4)))) */
    seq_t k = sqrt2;                      /* floor(2^(1/4)*n) */
    seq_t k4 = pow_seq(k, 4);             /* k^4 */
    return two_n4 - k4;                   /* 2*n^4 - k^4 */
}
