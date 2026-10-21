/*
 * A285198  Binomial coefficients C(9,n).
 * https://oeis.org/A285198
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = C(9,n) for 0 ≤ n ≤ 9 (otherwise 0)
 * Range:     u8 n <= 9, u16 n <= 9, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n > 9) return 0;
    seq_t res = 1;
    for (idx_t i = 1; i <= n; ++i)
    {
        /* Multiply by the falling factor (9 - i + 1) and divide by i,
           keeping the result integral at every step. */
        res = (res * ((seq_t)(9 - i + 1))) / ((seq_t)i);
    }
    return res;
}
