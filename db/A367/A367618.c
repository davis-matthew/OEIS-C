/*
 * A367618  a(n) is the unique k such that n is a comma-child of k in base 3,
 *          or -1 if k does not exist.
 * https://oeis.org/A367618
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, base
 * Formula:   a(n) = n - y - 3*((n - y) mod 3), where y is the most
 *            significant digit of n in base 3; returns -1 if the result is <=
 *            0
 * Range:     i8 n <= 128, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* find the most significant base‑3 digit of n (y) */
    idx_t t = n;
    while (t >= 3)
        t /= 3;
    seq_t y = (seq_t)t;          /* y is 1 or 2 */

    seq_t sn = (seq_t)n;
    seq_t m  = sn - y;           /* n - y */
    seq_t r  = m % (seq_t)3;      /* (n - y) mod 3 */
    seq_t res = sn - y - (seq_t)3 * r;

    if (res <= 0)
        return (seq_t)-1;
    return res;
}
