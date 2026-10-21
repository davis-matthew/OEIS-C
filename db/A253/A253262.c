/*
 * A253262  Expansion of (x + x^2 + x^3) / (1 - x + x^2 - x^3 + x^4) in powers
 *          of x.
 * https://oeis.org/A253262
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = a(n-1) - a(n-2) + a(n-3) - a(n-4) for n >= 4, with
 *            initial values a(0) = 0, a(1) = 1, a(2) = 2, a(3) = 2
 * Range:     i8 n <= 2500, i16 n <= 2500, i32 n <= 2500, i64 n <= 2500,
 *            i128 n <= 2500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t a_m1 = 0;   /* a(n-1) */
    seq_t a_m2 = 0;   /* a(n-2) */
    seq_t a_m3 = 0;   /* a(n-3) */
    seq_t a_m4 = 0;   /* a(n-4) */
    seq_t new_val = 0;

    for (idx_t i = 1; i <= n; ++i) {
        seq_t bi = 0;
        if (i == 1 || i == 2 || i == 3) bi = 1;   /* numerator contributes for n=1,2,3 */
        new_val = a_m1 - a_m2 + a_m3 - a_m4 + bi;
        a_m4 = a_m3;
        a_m3 = a_m2;
        a_m2 = a_m1;
        a_m1 = new_val;
    }
    return a_m1;  /* a(n) is stored in a_m1 after the loop */
}
