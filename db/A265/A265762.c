/*
 * A265762  Coefficient of x in minimal polynomial of the continued fraction
 *          [1^n,2,1,1,1,...], where 1^n means n ones.
 * https://oeis.org/A265762
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 2*a(n-1) + 2*a(n-2) - a(n-3), with a(0) = -3, a(1) = -5,
 *            a(2) = -15
 * Range:     i8 n <= 4, i16 n <= 10, i32 n <= 21, i64 n <= 44, i128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first three values directly */
    if (n == 0) return (seq_t)-3;
    if (n == 1) return (seq_t)-5;
    if (n == 2) return (seq_t)-15;

    /* Use three variables to store the last three computed values.
       Begin with a(0), a(1), a(2). */
    seq_t a0 = (seq_t)-3;
    seq_t a1 = (seq_t)-5;
    seq_t a2 = (seq_t)-15;
    seq_t next = a2; /* placeholder */

    /* Compute up to the desired n */
    for (idx_t i = 3; i <= n; ++i) {
        next = 2*a2 + 2*a1 - a0;   /* recurrence */
        a0 = a1;   /* shift window */
        a1 = a2;
        a2 = next;
    }

    return next;
}
