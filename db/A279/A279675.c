/*
 * A279675  Coefficients in the expansion of 1/([r] + [2r]x + [3r]x^2 + ...);
 *          [ ] = floor, r = 4/3.
 * https://oeis.org/A279675
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 15, i16 n <= 30, i32 n <= 62, i64 n <= 127,
 *            i128 n <= 254, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A279675:  a(n) = coefficient of x^n in 
 * (1 - x)(1 - x^3)/(1 + x + 2x^2)
 * Recurrence: a(n) = -a(n-1) - 2*a(n-2)   for n >= 5
 * Initial values: a(0)=1, a(1)=-2, a(2)=0, a(3)=3, a(4)=-2 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)-2;
    if (n == 2) return (seq_t)0;
    if (n == 3) return (seq_t)3;
    if (n == 4) return (seq_t)-2;

    seq_t a_prev2 = (seq_t)3;   /* a(3) */
    seq_t a_prev1 = (seq_t)-2;  /* a(4) */
    seq_t a_curr;

    for (idx_t i = 5; i <= n; ++i) {
        a_curr = -(a_prev1) - (a_prev2 * (seq_t)2);
        a_prev2 = a_prev1;
        a_prev1 = a_curr;
    }
    return a_prev1;
}
