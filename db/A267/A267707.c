/*
 * A267707  a(n) = A000217(A000217(n)+1).
 * https://oeis.org/A267707
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n*(n+1)/2+1)*(n*(n+1)/2+2)/2
 * Range:     u8 n <= 6, u16 n <= 26, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t T = (seq_t)n * ((seq_t)n + 1) / (seq_t)2;   /* triangular number T(n) */
    seq_t a_val = (T + 1) * (T + 2) / (seq_t)2;
    return a_val;
}
