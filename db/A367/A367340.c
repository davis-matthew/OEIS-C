/*
 * A367340  List of distinct (positive) numbers appearing in A367338.
 * https://oeis.org/A367340
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   search: a(n) = n-th smallest m such that m = A367338(k) for some
 *            k >= 1, where k = m - (10*x + msd(m)) with x = k mod 10 (only 10
 *            candidate k per m)
 * Range:     u8 n <= 200, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 80 terms through n = 9797 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static uint64_t A367340_child(uint64_t n)
{
    uint64_t x = n % 10;
    for (uint64_t y = 1; y <= 9; ++y) {
        uint64_t m = n + 10 * x + y;
        uint64_t t = m;
        while (t >= 10) t /= 10;
        if (t == y) return m;
    }
    return 0;
}

seq_t a(idx_t n)
{
    uint64_t count = 0;
    for (uint64_t m = 1;; ++m) {
        uint64_t t = m;
        while (t >= 10) t /= 10;
        uint64_t y = t;
        bool found = false;
        for (uint64_t x = 0; x <= 9 && !found; ++x) {
            uint64_t d = 10 * x + y;
            if (m <= d) continue;
            uint64_t k = m - d;
            if (k % 10 != x) continue;
            if (A367340_child(k) == m) found = true;
        }
        if (found) {
            ++count;
            if (count == (uint64_t)n) return (seq_t)m;
        }
    }
}
