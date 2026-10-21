/*
 * A288485  Expansion of (E_4(q) - 28*E_4(q^2) + 63*E_4(q^3) - 36*E_4(q^6)) /
 *          240.
 * https://oeis.org/A288485
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = sigma_3(n) - 28*sigma_3(n/2) + 63*sigma_3(n/3) -
 *            36*sigma_3(n/6), where sigma_3(m) = sum_{d|m} d^3
 * Range:     i8 n <= 3, i16 n <= 25, i32 n <= 854, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t sigma3(idx_t m)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = (idx_t)1; d <= m / d; ++d) {
        if (m % d == 0) {
            seq_t d3 = (seq_t)d * (seq_t)d * (seq_t)d;
            sum += d3;
            idx_t e = m / d;
            if (e != d) {
                seq_t e3 = (seq_t)e * (seq_t)e * (seq_t)e;
                sum += e3;
            }
        }
    }
    return sum;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* offset is 1, but we handle n=0 safely */
    seq_t res = sigma3(n);
    if ((n & 1) == 0) /* n%2==0 */
        res -= (seq_t)28 * sigma3(n / 2);
    if ((n % 3) == 0)
        res += (seq_t)63 * sigma3(n / 3);
    if ((n % 6) == 0)
        res -= (seq_t)36 * sigma3(n / 6);
    return res;
}
