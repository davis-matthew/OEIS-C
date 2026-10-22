/*
 * A380346  Number of corona for a hexagon of edge n with diamonds of side 1.
 * https://oeis.org/A380346
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = n^6 + 6*n^5 + 21*n^4 + 44*n^3 + 60*n^2 + 48*n + 18
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;
    seq_t r = 1;
    r = r * x + 6;
    r = r * x + 21;
    r = r * x + 44;
    r = r * x + 60;
    r = r * x + 48;
    r = r * x + 18;
    return r;
}
