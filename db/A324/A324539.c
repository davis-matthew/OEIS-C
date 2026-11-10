/*
 * A324539  Number of divisors d of n such that A276086(d) = (n/d).
 * https://oeis.org/A324539
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} [A276086(d) == n/d]; enumerates all divisor
 *            pairs of n
 * Range:     u8 n <= 100000, u16 n <= 100000, u32 n <= 100000,
 *            u64 n <= 100000, u128 n <= 100000, big any n
 * Validated: standard, pass, 136 terms through n = 9879 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Checks whether A276086(m) == t, stopping early once the product exceeds t. */
static bool A324539_eq(uint64_t m, uint64_t t)
{
    static const uint64_t pr[] = {2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71};
    uint64_t r = 1;
    int i = 0;
    while (m > 0) {
        if (i >= 20) return false;
        uint64_t p = pr[i++];
        uint64_t d = m % p;
        m /= p;
        for (uint64_t j = 0; j < d; ++j) {
            if (r > t / p) return false;
            r *= p;
        }
    }
    return r == t;
}

seq_t a(idx_t n)
{
    uint64_t N = (uint64_t)n;
    uint64_t cnt = 0;
    if (N == 0) return (seq_t)0;
    for (uint64_t m = 1; m <= N / m; ++m) {
        if (N % m == 0) {
            uint64_t q = N / m;
            if (A324539_eq(m, q)) cnt++;
            if (q != m && A324539_eq(q, m)) cnt++;
        }
    }
    return (seq_t)cnt;
}
