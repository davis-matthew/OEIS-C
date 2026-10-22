/*
 * A360036  Expansion of e.g.f. x*exp(x)*(sinh(x))^2.
 * https://oeis.org/A360036
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(3^(n-1) + (-1)^(n-1) - 2)/4 (a(0) = 0)
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 18, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    /* Compute 3^(n-1) */
    seq_t pow3 = (seq_t)1;
    for (idx_t i = 1; i < n; ++i)
        pow3 *= (seq_t)3;

    /* Compute val = 3^(n-1) - 1 for even (n-1), or 3^(n-1) - 3 for odd (n-1) */
    seq_t val;
    if (((n - 1) & (idx_t)1) == 0)         /* (n-1) even => (-1)^(n-1) = +1  */
        val = pow3 - (seq_t)1;             /* 3^(n-1) + 1 - 2 = 3^(n-1) - 1 */
    else
        val = pow3 - (seq_t)3;             /* 3^(n-1) - 1 - 2 = 3^(n-1) - 3 */

    return ((seq_t)n * val) / (seq_t)4;
}
