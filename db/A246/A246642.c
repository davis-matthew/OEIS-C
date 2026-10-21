/*
 * A246642  Sequence appearing in the curvature of a certain four-circle
 *          touching problem: (-3 + 5*A007805(n))/2.
 * https://oeis.org/A246642
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 19*a(n-1) - 19*a(n-2) + a(n-3), with a(0) = 1, a(1) = 41,
 *            a(2) = 761. Recurrence is derived from the series definition of
 *            A246642
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Constants for the recurrence: a(-2)=41, a(-1)=1, a(0)=1 */
    seq_t a_minus2 = 41; /* a(-2) */
    seq_t a_minus1 = 1;  /* a(-1) */
    seq_t a0 = 1;        /* a(0) */

    if (n == 0) {
        return a0;
    }

    /* Compute terms a(1) .. a(n) iteratively */
    for (idx_t i = 1; i <= n; ++i) {
        seq_t new_val = 19 * a0 - 19 * a_minus1 + a_minus2;
        /* Shift the previous terms for next iteration */
        a_minus2 = a_minus1;
        a_minus1 = a0;
        a0 = new_val;
    }

    return a0;
}
