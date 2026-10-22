/*
 * A309337  a(n) = n^3 if n odd, 3*n^3/4 if n even.
 * https://oeis.org/A309337
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = n^3 if n odd, 3*n^3/4 if n even
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 1625, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t x = (seq_t)n;
    x = x * x * x;          /* compute n^3 */
    if ((n & 1) == 0) {      /* even n */
        x = x * 3 / 4;       /* multiply by 3 and divide by 4 */
    }
    return x;
}
