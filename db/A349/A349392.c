/*
 * A349392  Dirichlet convolution of A126760 with tau (number of divisors
 *          function).
 * https://oeis.org/A349392
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} A126760(n/d) * A000005(d)
 * Range:     u8 n <= 279, u16 n <= 20000, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 98 terms through n = 9989 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A126760_a(idx_t n)
{
    seq_t result = 0;
    idx_t m = n;

    while (m > 0) {
        idx_t r = m % 6;
        if (r == 1) {
            result = ((seq_t)2 * ((seq_t)(m - 1) / 6)) + 1;
            break;
        }
        if (r == 5) {
            result = ((seq_t)2 * ((seq_t)(m - 5) / 6)) + 2;
            break;
        }
        if ((m & 1) == 0) {
            m >>= 1;
            continue;
        }
        if (m % 3 == 0) {
            m /= 3;
            continue;
        }
        break;
    }
    return result;
}

static seq_t A000005_a(idx_t n)
{
    seq_t result = 1;
    idx_t m = n;
    idx_t p;

    if (m == 0)
        return (seq_t)0;

    for (p = 2; p <= m / p; p++) {
        if (m % p == 0) {
            unsigned e = 0;
            while (m % p == 0) {
                m /= p;
                e++;
            }
            result *= (seq_t)(e + 1);
        }
    }
    if (m > 1)
        result *= 2;
    return result;
}

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t i;

    if (n == 0)
        return (seq_t)0;

    for (i = 1; i <= n / i; i++) {
        if (n % i == 0) {
            idx_t j = n / i;
            sum += A126760_a(j) * A000005_a(i);
            if (j != i)
                sum += A126760_a(i) * A000005_a(j);
        }
    }
    return sum;
}
