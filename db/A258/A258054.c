/*
 * A258054  Circle of fifths cycle (counterclockwise).
 * https://oeis.org/A258054
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + (5 * (n-1) mod 12)
 * Range:     u8 n <= 108, u16 n <= 108, u32 n <= 108, u64 n <= 108,
 *            u128 n <= 108, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* compute (n-1) mod 12 */
    idx_t r = (n - 1) % 12;
    /* multiply by 5 and reduce mod 12 */
    r = (r * 5) % 12;
    /* add 1 (the sequence is 1-indexed) */
    return (seq_t)(r + 1);
}
