/*
 * A305068  a(n) = 54*n - 18 (n>=1).
 * https://oeis.org/A305068
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 54*n - 18
 * Range:     u8 n <= 5, u16 n <= 1213, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute the value using seq_t arithmetic to avoid overflow. */
    return (seq_t)54 * (seq_t)n - (seq_t)18;
}
