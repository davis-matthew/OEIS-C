/*
 * A331190  Expansion of (-5*(9 - 6*x + 2*x^2))/(-1 + x)^3.
 * https://oeis.org/A331190
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*(5*n^2 + 19*n + 18)/2
 * Range:     u8 n <= 2, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    /* Compute B = 5*n*n + 19*n + 18 as nn*((5*nn)+19) + 18 */
    seq_t B = nn * ((seq_t)5 * nn + (seq_t)19) + (seq_t)18;
    /* Since B is even, divide by 2 before multiplying by 5 */
    return (seq_t)5 * (B / 2);
}
