/*
 * A382997  a(n) = -Sum_{d|n} phi(n/d) * (-n)^d.
 * https://oeis.org/A382997
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = -Sum_{d|n} phi(n/d) * (-n)^d * Equivalent to a(n) =
 *            -Sum_{k=1..n} (-n)^gcd(n,k)
 * Range:     i8 n <= 3, i16 n <= 5, i32 n <= 9, i64 n <= 15, i128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>

/* Compute Euler's totient φ(m) for 1 ≤ m ≤ 2^64-1 */
static seq_t totient(idx_t m)
{
    seq_t result = m;
    idx_t temp = m;
    if (temp % 2 == 0) {
        result -= result / 2;
        while (temp % 2 == 0)
            temp /= 2;
    }
    for (idx_t p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            result -= result / p;
            while (temp % p == 0)
                temp /= p;
        }
    }
    if (temp > 1)
        result -= result / temp;
    return result;
}

/* Fast exponentiation for seq_t: base^exp , exp ≥ 0 */
static seq_t pow_big(seq_t base, int exp)
{
    seq_t res = 1;
    while (exp > 0) {
        if (exp & 1)
            res *= base;
        base *= base;
        exp >>= 1;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n == 0)  /* not used, but defined for safety */
        return 0;

    seq_t sum = 0;
    idx_t root = (idx_t)sqrt((double)n);
    for (idx_t i = 1; i <= root; ++i) {
        if (n % i == 0) {
            /* divisor d1 = i */
            idx_t d1 = i;
            seq_t phi1 = totient(n / d1);
            seq_t pow1 = pow_big(-(seq_t)n, (int)d1);
            sum += phi1 * pow1;

            /* divisor d2 = n / i, skip if equal to d1 */
            idx_t d2 = n / i;
            if (d2 != d1) {
                seq_t phi2 = totient(n / d2);
                seq_t pow2 = pow_big(-(seq_t)n, (int)d2);
                sum += phi2 * pow2;
            }
        }
    }
    return -sum;
}
