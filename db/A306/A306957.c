/*
 * A306957  a(n) = n!*binomial(10,n).
 * https://oeis.org/A306957
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n!*binomial(10,n) = 10!/(10-n)! for 0 ≤ n ≤ 10, else 0
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n > 10)
        return (seq_t)0;

    seq_t res = 1;
    /* compute the product (10-n+1) * (10-n+2) * ... * 10 */
    for (idx_t i = 10 - n + 1; i <= 10; ++i)
        res *= (seq_t)i;

    return res;
}
