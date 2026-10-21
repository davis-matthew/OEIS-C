/*
 * A270700  Triangular Star of David numbers (the figurate number of triangles
 *          framing a hexagram: a(0) = 12; thereafter a(n) = 36*n + 6).
 * https://oeis.org/A270700
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 12; a(n) = 36*n + 6 for n > 0
 * Range:     u8 n <= 6, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)12;
    return ((seq_t)n) * 36 + 6;
}
