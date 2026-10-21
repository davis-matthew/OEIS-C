/*
 * A266708  Coefficient of x in minimal polynomial of the continued fraction
 *          [1^n,phi,1,1,1,...], where 1^n means n ones and phi = golden ratio
 *          = (1 + sqrt(5))/2.
 * https://oeis.org/A266708
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 2*a(n-1) + 2*a(n-2) - a(n-3), with initial values a(0)=0,
 *            a(1)=-10, a(2)=-18
 * Range:     i8 n <= 3, i16 n <= 9, i32 n <= 21, i64 n <= 44, i128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)-10;
    if (n == 2) return (seq_t)-18;

    seq_t a0 = (seq_t)0;        /* a(n-3)   */
    seq_t a1 = (seq_t)-10;      /* a(n-2)   */
    seq_t a2 = (seq_t)-18;      /* a(n-1)   */
    seq_t next;

    for (idx_t i = 3; i <= n; ++i)
    {
        next = 2 * a2 + 2 * a1 - a0; /* a(n)  */
        a0 = a1;
        a1 = a2;
        a2 = next;
    }
    return a2;
}
