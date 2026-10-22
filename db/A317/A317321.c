/*
 * A317321  Multiples of 21 and odd numbers interleaved.
 * https://oeis.org/A317321
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(2n) = 21*n; a(2n+1) = 2*n+1
 * Range:     u8 n <= 25, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n & 1U) {                 /* odd index → odd value */
        return (seq_t)n;
    } else {                      /* even index → multiple of 21 */
        return (seq_t)21 * ((seq_t)(n >> 1));
    }
}
