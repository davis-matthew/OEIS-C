/*
 * A308607  Number of (not necessarily maximal) cliques in the wheel graph on
 *          n vertices.
 * https://oeis.org/A308607
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*n-2 for n>4, a(4)=16
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == (idx_t)4)
        return (seq_t)16;
    if (n < (idx_t)4)
        return (seq_t)0; /* undefined offset */
    return (seq_t)n * (seq_t)4 - (seq_t)2;
}
