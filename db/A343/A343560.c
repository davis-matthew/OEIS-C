/*
 * A343560  a(n) = (n-1)*(4*n+1).
 * https://oeis.org/A343560
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n-1)*(4*n+1)
 * Range:     u8 n <= 8, u16 n <= 128, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n_val = (seq_t)n;
    seq_t term = (n_val - 1) * ((seq_t)4 * n_val + 1);
    return term;
}
