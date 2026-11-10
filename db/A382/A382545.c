/*
 * A382545  a(n) = A071324(n) - A000010(n).
 * https://oeis.org/A382545
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A071324(n) - A000010(n), where A071324(n) is the
 *            alternating sum of the divisors of n taken in decreasing order
 *            starting with n, and A000010 is Euler's totient
 * Range:     u8 n <= 545, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 116 terms through n = 9646 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A382545_alt(uint64_t n)
{
    uint64_t tau = 0, d;
    for (d = 1; d <= n / d; d++) {
        if (n % d == 0) {
            tau += (d == n / d) ? 1 : 2;
        }
    }
    seq_t pos = 0, neg = 0;
    uint64_t i = 0;
    for (d = 1; d <= n / d; d++) {
        if (n % d != 0) continue;
        i++;
        uint64_t big = n / d;
        if (d == big) {
            /* middle divisor, descending index i-1 */
            if ((i - 1) % 2 == 0) pos += (seq_t)d; else neg += (seq_t)d;
        } else {
            if ((i - 1) % 2 == 0) pos += (seq_t)big; else neg += (seq_t)big;
            if ((tau - i) % 2 == 0) pos += (seq_t)d; else neg += (seq_t)d;
        }
    }
    return pos - neg;
}

static uint64_t A382545_phi(uint64_t n)
{
    uint64_t result = n, m = n, p;
    for (p = 2; p <= m / p; p++) {
        if (m % p == 0) {
            while (m % p == 0) m /= p;
            result -= result / p;
        }
    }
    if (m > 1) result -= result / m;
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    uint64_t n64 = (uint64_t)n;
    seq_t alt = A382545_alt(n64);
    seq_t phi = (seq_t)A382545_phi(n64);
    return alt - phi;
}
