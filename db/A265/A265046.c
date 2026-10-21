/*
 * A265046  Coordination sequence for a 4.6.6 point in the 3-transitive tiling
 *          {4.6.6, 6.6.6, 6.6.6.6} of the plane by squares and dominoes
 *          (hexagons).
 * https://oeis.org/A265046
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*n for n ≥ 7; a(0)=1, a(1)=3, a(2)=5, a(3)=8, a(4)=13,
 *            a(5)=18, a(6)=23
 * Range:     u8 n <= 63, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* handle the initial part of the sequence explicitly */
    if (n < 7) {
        static const seq_t init[7] = {1, 3, 5, 8, 13, 18, 23};
        return init[n];
    }
    /* for n >= 7 the coordination sequence is 4*n */
    return (seq_t)n * 4;
}
