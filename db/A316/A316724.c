/*
 * A316724  Generalized 26-gonal (or icosihexagonal) numbers: m*(12*m - 11)
 *          with m = 0, +1, -1, +2, -2, +3, -3, ...
 * https://oeis.org/A316724
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(6*n + 11)/2 if n is even; * a(n) = (n + 1)*(6*n - 5)/2
 *            if n is odd
 * Range:     u8 n <= 9, u16 n <= 147, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t s = (seq_t)n;

    if ((n & 1ULL) == 0) {            /* n even */
        return s * ((seq_t)6 * s + (seq_t)11) / 2;
    } else {                          /* n odd */
        seq_t s1 = s + (seq_t)1;
        return s1 * ((seq_t)6 * s - (seq_t)5) / 2;
    }
}
