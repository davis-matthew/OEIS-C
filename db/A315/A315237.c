/*
 * A315237  Coordination sequence Gal.4.128.4 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315237
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; for n >= 1, a(n) = 4*n + (n%4==0 ? 8*(n/4) : 2 +
 *            4*(n/4))
 * Range:     u8 n <= 43, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) {
        return 1;
    }
    /* integer division by 4 */
    idx_t floor_div = n / 4;
    seq_t c;
    if ((n & 3) == 0) {          /* n % 4 == 0 */
        c = (seq_t)8 * (seq_t)floor_div;
    } else {                     /* n % 4 != 0 */
        c = (seq_t)2 + (seq_t)4 * (seq_t)floor_div;
    }
    return (seq_t)4 * (seq_t)n + c;
}
