/*
 * A344979  Numbers k such that A011772(k) is equal to A344878(k).
 * https://oeis.org/A344979
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k >= 1 with A011772(k) = A344878(k); found by a
 *            search over k
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Validated: partial, pass, 77 terms through n = 77 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* A344878(k), or 0 if it exceeds cap (A011772(k) < 2k, so larger can never match). */
static uint64_t A344878_capped(uint64_t k, uint64_t cap)
{
    uint64_t ans = 1;
    uint64_t m = k;
    uint64_t p;
    for (p = 2; p * p <= m || m > 1; p = (p == 2) ? 3 : p + 2) {
        if (p * p > m) p = m;           /* remaining cofactor is prime */
        if (m % p) continue;
        uint64_t pe = 1;
        while (m % p == 0) { m /= p; pe *= p; }
        if (p == 2) pe *= 2;
        uint64_t v = pe - 1;
        uint64_t x = ans, y = v;
        while (y) { uint64_t t = x % y; x = y; y = t; }
        ans = (ans / x) * v;
        if (ans > cap) return 0;
        if (m == 1) break;
    }
    return ans;
}

static int A011772_equals(uint64_t k, uint64_t L)
{
    if (L == 0) return 0;
    if ((L * (L + 1) / 2) % k != 0) return 0;
    for (uint64_t m = 1; m < L; ++m)
        if ((m * (m + 1) / 2) % k == 0) return 0;
    return 1;
}

seq_t a(idx_t n)
{
    uint64_t count = 0;
    uint64_t k = 0;
    while (count < (uint64_t)n) {
        ++k;
        uint64_t L = A344878_capped(k, 2 * k);
        if (A011772_equals(k, L)) ++count;
    }
    return (seq_t)k;
}
