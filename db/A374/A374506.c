/*
 * A374506  Expansion of 1/(1 - 2*x - 3*x^2)^(7/2).
 * https://oeis.org/A374506
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, a(1) = 7; a(n) = ((2*n+5)*a(n-1) + 3*(n+5)*a(n-2))/n
 *            for n ≥ 2
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 13, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)7;

    seq_t prev2 = (seq_t)1;          /* a(n-2) */
    seq_t prev1 = (seq_t)7;          /* a(n-1) */
    seq_t current;

    for (idx_t i = 2; i <= n; ++i)
    {
        seq_t si = (seq_t)i;

        /* term1 = (2*i + 5) * a(i-1) */
        seq_t term1 = ((seq_t)2 * si + (seq_t)5) * prev1;

        /* term2 = 3*(i + 5) * a(i-2) */
        seq_t term2 = ((seq_t)3 * (si + (seq_t)5)) * prev2;

        seq_t numer = term1 + term2;
        current = numer / si;

        /* shift for next iteration */
        prev2 = prev1;
        prev1 = current;
    }

    return prev1;
}
