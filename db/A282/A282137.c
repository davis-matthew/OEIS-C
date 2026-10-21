/*
 * A282137  Expansion of (24x^2-10x-1)/(16x^3-16x^2+x-1).
 * https://oeis.org/A282137
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = a(n-1) - 16*a(n-2) + 16*a(n-3), a(0)=1, a(1)=11, a(2)=-29
 * Range:     i8 n <= 2, i16 n <= 6, i32 n <= 14, i64 n <= 30, i128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 1;
    if (n == 1) return 11;
    if (n == 2) return -29;

    seq_t v0 = 1;    /* a(0) */
    seq_t v1 = 11;   /* a(1) */
    seq_t v2 = -29;  /* a(2) */
    seq_t v3 = 0;    /* current value */

    for (idx_t i = 3; i <= n; ++i) {
        v3 = v2 - (seq_t)16 * v1 + (seq_t)16 * v0;
        v0 = v1;
        v1 = v2;
        v2 = v3;
    }
    return v3;
}
