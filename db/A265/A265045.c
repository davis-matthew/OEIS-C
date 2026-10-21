/*
 * A265045  Coordination sequence for a 6.6.6 point in the 3-transitive tiling
 *          {4.6.6, 6.6.6, 6.6.6.6} of the plane by squares and dominoes
 *          (hexagons).
 * https://oeis.org/A265045
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*n (for n >= 7); a(0)=1, a(1)=3, a(2)=7, a(3)=11,
 *            a(4)=14, a(5)=18, a(6)=23
 * Range:     u8 n <= 63, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    static const seq_t table[7] = {
        (seq_t)1,
        (seq_t)3,
        (seq_t)7,
        (seq_t)11,
        (seq_t)14,
        (seq_t)18,
        (seq_t)23
    };
    if (n < 7) {
        return table[n];
    }
    return (seq_t)n * ((seq_t)4);
}
