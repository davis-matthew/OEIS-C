/*
 * A385369  Expansion of e.g.f. x + sqrt(x^2 + 1).
 * https://oeis.org/A385369
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(0)=a(1)=1; for odd n>1, a(n)=0; for n=2m, a(n)=(-1)^{m-1} *
 *            (2m-1)!! * (2m-3)!! (double factorials of odd numbers)
 * Range:     i8 n <= 7, i16 n <= 9, i32 n <= 15, i64 n <= 23, i128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* base cases */
    if (n == 0 || n == 1)
        return (seq_t)1;

    /* odd indices (except 1) are zero */
    if ((n & 1ULL) == 1ULL)
        return (seq_t)0;

    /* even index: n = 2*m */
    idx_t m = n / 2;

    /* compute odd factorials up to m and m-1 */
    seq_t prev = (seq_t)1; /* oddfac[0] = 1 */
    seq_t curr = (seq_t)1; /* will become oddfac[1] ... oddfac[m] */

    for (idx_t k = 1; k <= m; ++k)
    {
        prev = curr;
        curr = curr * (seq_t)(2 * k - 1);
    }

    /* result = oddfac[m] * oddfac[m-1] with sign */
    seq_t result = curr * prev;
    if ((m & 1ULL) == 0)  /* m even -> negative sign */
        result = -result;

    return result;
}
