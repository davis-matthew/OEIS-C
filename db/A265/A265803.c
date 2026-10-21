/*
 * A265803  Coefficient of x in minimal polynomial of the continued fraction
 *          [1^n,4,1,1,1,...], where 1^n means n ones.
 * https://oeis.org/A265803
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 2*a(n-1) + 2*a(n-2) - a(n-3) with a(0) = -7, a(1) = -29,
 *            a(2) = -67
 * Range:     i8 n <= 2, i16 n <= 8, i32 n <= 19, i64 n <= 42, i128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle the initial values explicitly */
    if (n == 0) return (seq_t)-7;
    if (n == 1) return (seq_t)-29;
    if (n == 2) return (seq_t)-67;

    /* iterate the linear recurrence up to the requested index */
    seq_t a0 = (seq_t)-7;   /* a(n-3)   */
    seq_t a1 = (seq_t)-29;  /* a(n-2)   */
    seq_t a2 = (seq_t)-67;  /* a(n-1)   */
    seq_t a3;               /* a(n)     */

    for (idx_t i = 3; i <= n; ++i)
    {
        a3 = (seq_t)(2 * a2 + 2 * a1 - a0);   /* recurrence */
        a0 = a1;
        a1 = a2;
        a2 = a3;
    }
    return a2;
}
