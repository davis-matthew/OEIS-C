/*
 * A286591  Compound filter: a(n) = P(A009191(n), A009194(n)), where P(n,k) is
 *          sequence A000027 used as a pairing function.
 * https://oeis.org/A286591
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2 + (g+h)^2 - g - 3*h)/2 where g = gcd(n,d(n))
 *            (A009191), h = gcd(n,sigma(n)) (A009194)
 * Range:     u8 n <= 27, u16 n <= 495, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 123 terms through n = 9072 (2026-11-08)
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

static idx_t A009191_divisor_count(idx_t n)
{
    idx_t count = 1;
    idx_t m = n;
    for (idx_t p = 2; p <= m / p; ++p) {
        if (m % p == 0) {
            idx_t e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            count *= (e + 1);
        }
    }
    if (m > 1) count *= 2;
    return count;
}

static seq_t A009191_a(idx_t n)
{
    if (n == 0) return 0;
    seq_t d = (seq_t)A009191_divisor_count(n);
    return gcd((seq_t)n, d);
}

static seq_t A009194_sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d) sum += (seq_t)d2;
        }
    }
    return sum;
}

static seq_t A009194_a(idx_t n)
{
    if (n == 0) return 0;
    seq_t s = A009194_sigma_of(n);
    return gcd((seq_t)n, s);
}

seq_t a(idx_t n)
{
    seq_t x = A009191_a(n);
    seq_t y = A009194_a(n);
    seq_t s = x + y;
    return (seq_t)((s * s + 2 - x - 3 * y) / 2);
}
