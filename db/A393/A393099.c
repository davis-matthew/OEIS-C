/*
 * A393099  Number of 4-stack polyominoes having semiperimeter equal to n.
 * https://oeis.org/A393099
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = D_{n-2} - 6·D_{n-3} + 9·D_{n-4} with D_0 = 1 and D_k =
 *            2·D_{k-1} + (2k+1)·binomial(2k,k)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;

    seq_t D_nm4 = 0;      /* D_{-2} */
    seq_t D_nm3 = 0;      /* D_{-1} */
    seq_t D_nm2 = 1;      /* D_0 */

    for (idx_t k = 1; k <= n - 2; ++k)
    {
        seq_t ck = binomial(2 * k, k);           /* C(2k, k)   */
        ck = ck * (seq_t)(2 * k + 1);            /* c_k = (2k+1)*C(2k,k) */
        seq_t D_curr = (seq_t)(2 * D_nm2 + ck);   /* D_k = 2*D_{k-1} + c_k */

        /* shift window */
        D_nm4 = D_nm3;
        D_nm3 = D_nm2;
        D_nm2 = D_curr;
    }

    seq_t result = D_nm2 - (seq_t)(6 * D_nm3) + (seq_t)(9 * D_nm4);
    return result;
}
