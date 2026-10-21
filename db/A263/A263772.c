/*
 * A263772  Perimeters of integer-sided scalene triangles.
 * https://oeis.org/A263772
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 9, a(n) = n + 9 for n > 1
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n <= 1)
        return (seq_t)9;
    return (seq_t)n + (seq_t)9;
}
