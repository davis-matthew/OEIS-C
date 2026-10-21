/*
 * A268152  A double binomial sum involving absolute values.
 * https://oeis.org/A268152
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 4^(2*n-1)*n*(36*n^3-84*n^2+67*n-17) with a(0)=0
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 26,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    seq_t tn = (seq_t)n;
    seq_t t2 = tn * tn;          /* n^2 */
    seq_t t3 = t2 * tn;          /* n^3 */

    /* (36*n^3 + 67*n) - (84*n^2 + 17)  : all intermediate results are positive */
    seq_t A = 36 * t3 + 67 * tn;
    seq_t B = 84 * t2 + (seq_t)17;
    seq_t inner = A - B;          /* inner = 36*n^3 - 84*n^2 + 67*n - 17 */

    seq_t coeff = tn * inner;     /* n * inner */
    seq_t pow = pow2(4 * n - 2);  /* 4^(2*n-1) = 2^(4*n-2) */

    return pow * coeff;
}
