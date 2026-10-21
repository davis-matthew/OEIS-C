/*
 * A284344  Sum of the divisors of n that are not divisible by 10.
 * https://oeis.org/A284344
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sigma(n) - (sigma(2^a)-1)*(sigma(5^b)-1)*sigma(m) * where
 *            n = 2^a * 5^b * m and gcd(m,10)=1. * sigma(p^e) = 1 + p + p^2 +
 *            … + p^e. For primes p>1, * sigma(p^e) is computed by repeated
 *            multiplication. All * operations are performed in the seq_t type
 * Range:     u8 n <= 107, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t sigma_prime_power(idx_t p, idx_t e)
{
    seq_t sum = 1;   /* term for p^0 */
    seq_t pow  = 1;
    for (idx_t i = 0; i < e; ++i) {
        pow *= p;
        sum += pow;
    }
    return sum;
}

static seq_t sigma_of(idx_t n)
{
    seq_t result = 1;
    idx_t m = n;

    for (idx_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            idx_t e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            result *= sigma_prime_power(p, e);
        }
    }
    if (m > 1) {   /* remaining prime factor */
        result *= sigma_prime_power(m, 1);
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    idx_t m = n;
    idx_t aExp = 0, bExp = 0;

    /* Remove factors of 2 and 5 */
    while ((m & 1ULL) == 0) {  /* m % 2 == 0 */
        ++aExp;
        m >>= 1;               /* m /= 2 */
    }
    while (m % 5 == 0) {
        ++bExp;
        m /= 5;
    }

    /* sigma(n) */
    seq_t sigmaN = sigma_of(n);

    /* sigma(2^aExp), sigma(5^bExp) */
    seq_t sigma2 = sigma_prime_power(2ULL, aExp);
    seq_t sigma5 = sigma_prime_power(5ULL, bExp);

    /* sigma(m) */
    seq_t sigmaM = sigma_of(m);

    /* Sum of divisors divisible by 10 */
    seq_t sumDiv10 = (sigma2 - 1) * (sigma5 - 1) * sigmaM;

    return sigmaN - sumDiv10;
}
