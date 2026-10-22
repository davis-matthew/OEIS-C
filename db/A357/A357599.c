/*
 * A357599  Expansion of e.g.f. sinh(2 * log(1+x)) / 2.
 * https://oeis.org/A357599
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (-1)^(n+1) * (n+1)! / 4 for n > 2 a(0)=0, a(1)=1, a(2)=-1
 * Range:     i8 n <= 4, i16 n <= 7, i32 n <= 12, i64 n <= 19, i128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 0;
    if (n == 1) return 1;
    if (n == 2) return -1;
    seq_t val = 6;          /* a(3) */
    for (idx_t i = 4; i <= n; ++i) {
        /* multiply by negative (i+1) */
        val = -( (seq_t)(i + 1) ) * val;
    }
    return val;
}
