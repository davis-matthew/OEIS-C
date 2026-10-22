/*
 * A309338  a(n) = n^4 if n odd, 7*n^4/8 if n even.
 * https://oeis.org/A309338
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = n^4 if n odd; a(n) = 7*n^4/8 if n even
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 255, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t n_t = (seq_t)n;
    seq_t n4 = n_t * n_t * n_t * n_t;
    if ((n & 1) == 0) {        /* even */
        n4 = n4 * 7 / 8;
    }
    return n4;
}
