/*
 * A281200  Number of n X 3 0..1 arrays with no element equal to more than one
 *          of its horizontal and antidiagonal neighbors, with the exception
 *          of exactly one element, and with new values introduced in order 0
 *          sequentially upwards.
 * https://oeis.org/A281200
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7*(n-1)*2^(n-1) (for n>1, a(1)=1)
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 25, u64 n <= 56, u128 n <= 119,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;                     /* a(1) = 1 */

    /* Compute 2^(n-1) by repeated left–shift. */
    seq_t power = (seq_t)2;                   /* 2^1 */
    idx_t k = n - 1;
    for (idx_t i = 1; i < k; ++i)             /* shift (k-1) times */
        power <<= 1;

    /* a(n) = power * (n-1) * 7  */
    seq_t res = power * (seq_t)k;
    res *= (seq_t)7;
    return res;
}
