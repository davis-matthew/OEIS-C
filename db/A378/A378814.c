/*
 * A378814  a(n) = round(n/(A000005(A071904(n))-2)).
 * https://oeis.org/A378814
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = round(n/(A000005(A071904(n))-2)), rounding halves up;
 *            A071904(n) is the n-th odd composite, found by trial-division
 *            search
 * Range:     u8 n <= 274, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 87 terms through n = 9745 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static bool A071904_isprime(uint64_t n)
{
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    for (uint64_t d = 3; d <= n / d; d += 2)
        if (n % d == 0) return false;
    return true;
}

static uint64_t A000005_tau(uint64_t m)
{
    uint64_t result = 1, p;
    for (p = 2; p <= m / p; p++) {
        if (m % p == 0) {
            unsigned e = 0;
            while (m % p == 0) { m /= p; e++; }
            result *= (e + 1);
        }
    }
    if (m > 1) result *= 2;
    return result;
}

seq_t a(idx_t n)
{
    uint64_t cnt = 0, m = 7, nn = (uint64_t)n;
    if (nn == 0) return (seq_t)0;
    while (cnt < nn) {
        m += 2;
        if (!A071904_isprime(m)) cnt++;
    }
    uint64_t d = A000005_tau(m) - 2;
    return (seq_t)((2 * nn + d) / (2 * d));
}
