/*
 * A274268  Expansion of e.g.f. (1 + x)^4*log(1 + x).
 * https://oeis.org/A274268
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (-1)^(n-1) * 24 * (n - 5)! for n >= 5 with a(1)=1,
 *            a(2)=7, a(3)=26, a(4)=50, a(5)=24. This is the explicit closed
 *            form of OEIS entry A274268
 * Range:     i8 n <= 7, i16 n <= 11, i32 n <= 16, i64 n <= 24, i128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* first five terms are fixed */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)7;
    if (n == 3) return (seq_t)26;
    if (n == 4) return (seq_t)50;
    if (n == 5) return (seq_t)24;

    /* for n >= 5 compute factorial of (n-5) */
    idx_t k = n - 5;
    seq_t fact = (seq_t)1;
    for (idx_t i = 1; i <= k; ++i)
        fact *= (seq_t)i;          /* factorial computed multiplicatively */

    /* compute sign: positive for odd n, negative for even n */
    seq_t result = (seq_t)24 * fact;
    if ((n & 1) == 0)   /* n even -> negative sign */
        result = -result;
    return result;
}
