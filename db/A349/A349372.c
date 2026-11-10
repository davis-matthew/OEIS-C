/*
 * A349372  Dirichlet convolution of Kimberling's paraphrase, A003602, with
 *          tau (number of divisors, A000005).
 * https://oeis.org/A349372
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} A003602(n/d) * A000005(d)
 * Range:     u8 n <= 125, u16 n <= 20000, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 96 terms through n = 9460 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A003602_a(idx_t n) {
    while ((n & 1) == 0) {
        n >>= 1;
    }
    return (seq_t)((n + 1) >> 1);
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

seq_t a(idx_t n) {
    seq_t sum = 0;
    idx_t d;
    if (n == 0)
        return (seq_t)0;
    for (d = 1; d <= n / d; d++) {
        if (n % d == 0) {
            idx_t e = n / d;
            sum += A003602_a(e) * A000005_a(d);
            if (e != d)
                sum += A003602_a(d) * A000005_a(e);
        }
    }
    return sum;
}
