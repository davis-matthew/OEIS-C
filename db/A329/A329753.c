/*
 * A329753  Doubly square pyramidal numbers.
 * https://oeis.org/A329753
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A000330(A000330(n))
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 18, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Validated: partial, pass, 28 terms through n = 27 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline seq_t square_pyramid(seq_t n) {
    seq_t a = n;
    seq_t b = n + 1;
    seq_t c = 2 * n + 1;

    /* divide by 3 */
    if (a % 3 == 0) {
        a /= 3;
    } else if (b % 3 == 0) {
        b /= 3;
    } else {
        c /= 3;
    }

    /* divide by 2 */
    if ((a & 1) == 0) {
        a /= 2;
    } else if ((b & 1) == 0) {
        b /= 2;
    } else {
        c /= 2;
    }

    return a * b * c;
}
/* A000330(n) = n(n+1)(2n+1)/6, the square pyramidal number */

seq_t a(idx_t n) {
    seq_t n_seq = (seq_t)n;
    seq_t t = square_pyramid(n_seq);
    return square_pyramid(t);
}
