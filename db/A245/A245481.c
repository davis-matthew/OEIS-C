/*
 * A245481  Numbers k such that the k-th cyclotomic polynomial has a root mod
 *          13.
 * https://oeis.org/A245481
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = div[(n-1)%6] * 13^((n-1)/6) where div = {1,2,3,4,6,12}
 * Range:     u8 n <= 13, u16 n <= 26, u32 n <= 52, u64 n <= 104,
 *            u128 n <= 208, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Divisors of 12, in ascending order */
    static const seq_t divs[6] = { (seq_t)1, (seq_t)2, (seq_t)3,
                                   (seq_t)4, (seq_t)6, (seq_t)12 };

    /* n is guaranteed >= 1; guard against accidental zero on some builds */
    if (n == 0)
        return 0;

    /* Convert index to zero‑based to ease computation */
    idx_t idx   = n - 1;
    idx_t block = idx / 6;          /* equals the exponent j */
    idx_t pos   = idx % 6;          /* position in the 6‑element divisor list */

    /* Compute 13^j in seq_t */
    seq_t pow13 = (seq_t)1;
    for (idx_t i = 0; i < block; ++i)
        pow13 *= (seq_t)13;

    return divs[pos] * pow13;
}
