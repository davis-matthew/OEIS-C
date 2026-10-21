/*
 * A251701  a(n) = 3^n + n^2.
 * https://oeis.org/A251701
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3^n + n^2
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 20, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    seq_t pow_val = pow3(n);                     /* 3^n */
    seq_t nsq = (seq_t)n * (seq_t)n;             /* n^2 */
    return pow_val + nsq;                       /* a(n) = 3^n + n^2 */
}
