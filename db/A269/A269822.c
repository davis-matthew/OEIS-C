/*
 * A269822  Number of n X 1 0..4 arrays with some element plus some
 *          horizontally, diagonally, antidiagonally or vertically adjacent
 *          neighbor totalling four exactly once.
 * https://oeis.org/A269822
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*4^(n-2)*(n-1) for n>=2, a(1) = 0
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 30, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2)                /* a(1) and any n<1 */
        return 0;

    seq_t pow4 = 1;           /* compute 4^(n-2) */
    for (idx_t i = 0; i < n - 2; ++i)
        pow4 *= (seq_t)4;

    seq_t res = pow4;         /* 4^(n-2) */
    res *= (seq_t)(n - 1);    /* * (n-1) */
    res *= (seq_t)5;          /* * 5 */

    return res;
}
