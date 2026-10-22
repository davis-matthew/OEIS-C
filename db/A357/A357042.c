/*
 * A357042  The sum of the numbers of the central diamond of the
 *          multiplication table [1..k] X [1..k] for k=2*n-1.
 * https://oeis.org/A357042
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2*(2*n^2 - 2*n + 1)
 * Range:     u8 n <= 3, u16 n <= 13, u32 n <= 215, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N  = (seq_t)n;
    seq_t N2 = N * N;                        /* n^2 */
    seq_t term = ((seq_t)2 * N2) - ((seq_t)2 * N) + (seq_t)1;
    return N2 * term;                        /* n^2 * (2*n^2 - 2*n + 1) */
}
