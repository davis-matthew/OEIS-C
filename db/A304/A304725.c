/*
 * A304725  a(n) = n^4 + 8*n^3 + 20*n^2 + 16*n + 2.
 * https://oeis.org/A304725
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^4 + 8*n^3 + 20*n^2 + 16*n + 2
 * Range:     u8 n <= 2, u16 n <= 14, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert the index to the element type once */
    seq_t x = (seq_t)n;
    return x*x*x*x + (seq_t)8*x*x*x + (seq_t)20*x*x + (seq_t)16*x + (seq_t)2;
}
