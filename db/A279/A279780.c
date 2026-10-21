/*
 * A279780  Coefficients in the expansion of 1/([r] + [2r]x + [3r]x^2 + ...);
 *          [ ] = floor, r = 8/5.
 * https://oeis.org/A279780
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   sequence A279780 * * G.f.: 1/([r] + [2r]x + [3r]x² + …), [ ] =
 *            floor, r = 8/5. * Equivalent rational form: * (1 - x)(1 - x⁵) /
 *            (1 + 2x + x² + 2x³ + 2x⁴). * * This yields the linear recurrence
 *            (valid for all n ≥ 5): * a(n) = -2*a(n-1) - a(n-2) - 2*a(n-3) -
 *            2*a(n-4) + c(n) * where c(n) = 0 except *...
 * Range:     i8 n <= 7, i16 n <= 18, i32 n <= 39, i64 n <= 81, i128 n <= 165,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first five terms directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)(-3);
    if (n == 2) return (seq_t)5;
    if (n == 3) return (seq_t)(-9);
    if (n == 4) return (seq_t)17;

    /* Initialize a(n-4) … a(n-1) for the recurrence.  For n = 5
       the current stored values correspond to a(1) … a(4). */
    seq_t a_nm4 = (seq_t)(-3);   /* a(1) */
    seq_t a_nm3 = (seq_t)5;      /* a(2) */
    seq_t a_nm2 = (seq_t)(-9);   /* a(3) */
    seq_t a_nm1 = (seq_t)17;     /* a(4) */
    seq_t cur   = 0;             /* a(n) being computed */

    for (idx_t i = 5; i <= n; ++i) {
        seq_t c = 0;             /* constant term c(n) */
        if (i == 5) c = (seq_t)(-1);
        else if (i == 6) c = (seq_t)1;

        /* Recurrence: a(n) = -2*a(n-1) - a(n-2) - 2*a(n-3) - 2*a(n-4) + c(n) */
        cur = -2 * a_nm1 - a_nm2 - 2 * a_nm3 - 2 * a_nm4 + c;

        /* Shift the window for the next iteration. */
        a_nm4 = a_nm3;
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = cur;
    }

    return cur;
}
