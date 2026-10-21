/*
 * A241850  a(n) = n^2 + 20.
 * https://oeis.org/A241850
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2 + 20
 * Range:     u8 n <= 15, u16 n <= 255, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS sequence A241850: a(n) = n^2 + 20 for n ≥ 0. */
seq_t a(idx_t n)
{
    /* Ensure the multiplication is performed in seq_t to avoid overflow in the intermediate. */
    return ((seq_t)n * (seq_t)n) + (seq_t)20;
}
