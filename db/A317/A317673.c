/*
 * A317673  Moebius transform of A129502.
 * https://oeis.org/A317673
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy, mult
 * Formula:   a(n) = (e0 + 1) * (-1)^k * e0 = exponent of 2 in n (0 if n odd)
 *            * k = number of odd prime divisors of n, each appearing to power
 *            1 * If any odd prime divides n to power ≥ 2, a(n) = 0. * a(n) is
 *            multiplicative. * a(1) = 1, a(2) = 2, a(3) = –1, a(4) = 3, … as
 *            in OEIS A317673
 * Range:     i8 n <= 1024, i16 n <= 1024, i32 n <= 1024, i64 n <= 1024,
 *            i128 n <= 1024, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* values below the offset are irrelevant */

    idx_t  m = n;                          /* work on a copy of n */
    unsigned e0 = 0;                       /* exponent of 2 */
    int k = 0;                             /* count of odd prime divisors */

    /* Count the power of 2 */
    while ((m & 1ULL) == 0) {
        ++e0;
        m >>= 1;
    }

    /* Factor the remaining odd part */
    for (idx_t d = 3; d <= m / d; d += 2) {
        if (m % d == 0) {
            unsigned exp = 0;
            do {
                m /= d;
                ++exp;
            } while (m % d == 0);

            if (exp >= 2)              /* odd prime squared – result is zero */
                return (seq_t)0;

            ++k;                       /* odd prime of exponent 1 */
        }
    }

    /* If a prime factor remains > 1, it is an odd prime of exponent 1 */
    if (m > 1) ++k;

    seq_t res = (seq_t)(e0 + 1);
    if (k & 1) res = -res;
    return res;
}
