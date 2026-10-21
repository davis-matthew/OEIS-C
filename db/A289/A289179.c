/*
 * A289179  Edge count of the n X n white bishop graph.
 * https://oeis.org/A289179
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((-1 + n)*(-3 + 3*(-1)^n - 2*n + 4*n^2))/12
 * Range:     u8 n <= 9, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* safety for out-of-range */
    int64_t nn = (int64_t)n;             /* convert for signed arithmetic */
    int64_t pow_minus1 = (n & 1) ? -1 : 1;/* (-1)^n */
    int64_t inner = -3 + 3 * pow_minus1 - 2 * nn + 4 * nn * nn;
    int64_t prod   = (nn - 1) * inner;
    int64_t res    = prod / 12;
    return (seq_t)res;
}
