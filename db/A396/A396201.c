/*
 * A396201  a(x) = psi * phi^(-x) + (1 - psi) * (-phi)^x, where phi = (1 +
 *          sqrt(5)) / 2 and psi = phi^4 / sqrt(5) - 1, evaluated at the
 *          nonnegative integers.
 * https://oeis.org/A396201
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = a(n-2) - a(n-1), a(0)=1, a(1)=3
 * Range:     i8 n <= 9, i16 n <= 21, i32 n <= 44, i64 n <= 90, i128 n <= 182,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (The recurrence holds for all n >= 2.) */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;
    seq_t a_nm2 = (seq_t)1;   /* a(0) */
    seq_t a_nm1 = (seq_t)3;   /* a(1) */
    seq_t a_n;
    for (idx_t i = 2; i <= n; ++i) {
        a_n = a_nm2 - a_nm1; /* a(i) = a(i-2) - a(i-1) */
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_n;
}
