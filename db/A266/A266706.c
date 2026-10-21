/*
 * A266706  Coefficient of x in minimal polynomial of the continued fraction
 *          [1^n,sqrt(5),1,1,1,...], where 1^n means n ones.
 * https://oeis.org/A266706
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 3*a(n-1) - a(n-2) - 23*(-1)^n Initial values: a(1)=1,
 *            a(2)=-23, a(3)=-45
 * Range:     i8 n <= 3, i16 n <= 9, i32 n <= 21, i64 n <= 44, i128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) /* offset undefined, just return 0 */
        return 0;

    if (n == 1)
        return (seq_t)1;
    if (n == 2)
        return (seq_t)-23;
    if (n == 3)
        return (seq_t)-45;

    seq_t prev2 = (seq_t)-23; /* a(2)  */
    seq_t prev1 = (seq_t)-45; /* a(3)  */
    seq_t curr  = 0;

    for (idx_t i = 4; i <= n; ++i) {
        seq_t sign = ((i & 1) == 0) ? (seq_t)1 : (seq_t)-1;
        curr = (seq_t)3 * prev1 - prev2 - (seq_t)23 * sign;
        prev2 = prev1;
        prev1 = curr;
    }

    return curr;
}
