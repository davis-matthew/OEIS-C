/*
 * A269132  a(n) = n + floor(n*(2*n+1)/5).
 * https://oeis.org/A269132
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + floor(n*(2*n+1)/5)
 * Range:     u8 n <= 23, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t ns = (seq_t)n;                      /* index as seq_t */
    seq_t term = (ns * ((seq_t)2 * ns + (seq_t)1)) / (seq_t)5;
    return ns + term;
}
