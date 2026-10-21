/*
 * A248572  a(n) = 29*n + 1.
 * https://oeis.org/A248572
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 29*n + 1
 * Range:     u8 n <= 8, u16 n <= 2259, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute 29*n + 1 in the target type seq_t. */
    return (seq_t)n * 29 + 1;
}
