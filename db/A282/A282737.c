/*
 * A282737  Expansion of (x^6 - x^4 + x^3 - x - 1)/((x - 1)^3*(x + 1)^2).
 * https://oeis.org/A282737
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^2 + 14*n)/8 for even n ≥ 2; a(n) = (n^2 + 12*n - 5)/8
 *            for odd n ≥ 3; a(0) = 1; a(1) = 2;
 * Range:     u8 n <= 39, u16 n <= 717, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;

    if ((n & 1ULL) == 0) /* even */
        return (((seq_t)n) * ((seq_t)n) + (seq_t)14 * ((seq_t)n)) / (seq_t)8;
    else /* odd */
        return (((seq_t)n) * ((seq_t)n) + (seq_t)12 * ((seq_t)n) - (seq_t)5) / (seq_t)8;
}
