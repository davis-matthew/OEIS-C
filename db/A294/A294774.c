/*
 * A294774  a(n) = 2*n^2 + 2*n + 5.
 * https://oeis.org/A294774
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n*n + 2*n + 5
 * Range:     u8 n <= 10, u16 n <= 180, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert the index to the sequence type before multiplication
       to avoid overflows in the intermediate result when seq_t is
       a larger integer type (e.g., unsigned __int128). */
    seq_t nn = (seq_t)n;
    return (nn * nn) * 2 + nn * 2 + 5;
}
