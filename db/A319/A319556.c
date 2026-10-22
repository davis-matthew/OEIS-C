/*
 * A319556  a(n) gives the alternating sum of length n, starting at n: n -
 *          (n+1) + (n+2) - ... + (-1)^(n+1) * (2n-1).
 * https://oeis.org/A319556
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = Sum_{k=n}^{2*n-1} (-1)^(n-k) * k For odd n: a(n) =
 *            3*((n+1)/2) - 2 For even n: a(n) = -(n/2)
 * Range:     i8 n <= 86, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if ((n & 1ULL) != 0)   /* n is odd */
    {
        seq_t k = (seq_t)((n + 1) / 2);  /* k = (n+1)/2 */
        return (seq_t)(3 * k - 2);
    }
    else                   /* n is even */
    {
        seq_t k = (seq_t)(n / 2);        /* k = n/2 */
        return -k;
    }
}
