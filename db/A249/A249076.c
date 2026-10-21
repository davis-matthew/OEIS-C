/*
 * A249076  a(n) = (n*(n+1))^6.
 * https://oeis.org/A249076
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n*(n+1))^6
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 39, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t t = (seq_t)n;
    t = t * (((seq_t)n) + 1);
    seq_t t2 = t * t;      /* t^2 */
    seq_t t4 = t2 * t2;    /* t^4 */
    seq_t t6 = t4 * t2;    /* t^6 */
    return t6;
}
