/*
 * A269877  a(n) =
 *          2^(4*n-3)*n*(2*n-1)*(900*n^4-4500*n^3+8895*n^2-8055*n+2764), a
 *          closed form for a double binomial sum involving absolute values.
 * https://oeis.org/A269877
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) =
 *            2^(4*n-3)*n*(2*n-1)*(900*n^4-4500*n^3+8895*n^2-8055*n+2764)
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 3, u64 n <= 9, u128 n <= 23,
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

    seq_t sn = (seq_t)n;

    /* 2^(4*n-3) */
    seq_t pow_term = pow2(4 * n - 3);

    /* n*(2*n-1) */
    seq_t n_times = sn * (sn * 2 - 1);

    /* polynomial 900*n^4 - 4500*n^3 + 8895*n^2 - 8055*n + 2764 */
    seq_t n2 = sn * sn;            /* n^2 */
    seq_t n3 = n2 * sn;            /* n^3 */
    seq_t n4 = n2 * n2;            /* n^4 */

    seq_t poly = (seq_t)900 * n4;
    poly -= (seq_t)4500 * n3;
    poly += (seq_t)8895 * n2;
    poly -= (seq_t)8055 * sn;
    poly += (seq_t)2764;

    return pow_term * n_times * poly;
}
