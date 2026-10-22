/*
 * A319273  Signed sum over the prime multiplicities of n.
 * https://oeis.org/A319273
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Σ_{i≥1} (-1)^{i-1} * exp_i, where exp_i are the exponents
 *            in the prime factorization of n sorted by increasing primes.
 *            a(1) is defined to be 1
 * Range:     i8 n <= 9895, i16 n <= 9895, i32 n <= 9895, i64 n <= 9895,
 *            i128 n <= 9895, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;          /* a(1) = 1 */

    seq_t res = 0;
    uint64_t m = (uint64_t)n;
    int sign = 1;                         /* + for the smallest prime, then alternate */

    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            int exp = 0;
            while (m % p == 0) {
                m /= p;
                ++exp;
            }
            res += (seq_t)(sign * exp);
            sign = -sign;                /* alternate sign for next prime */
        }
    }
    if (m > 1) {                         /* remaining prime factor */
        res += (seq_t)(sign * 1);
    }
    return res;
}
