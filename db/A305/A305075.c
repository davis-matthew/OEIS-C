/*
 * A305075  a(n) = 32*n - 24 (n>=1).
 * https://oeis.org/A305075
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 32*n - 24
 * Range:     u8 n <= 8, u16 n <= 2048, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute 32*n as seq_t to avoid overflow on the intermediate. */
    return ((seq_t)32 * (seq_t)n) - 24;
}
