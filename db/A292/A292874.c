/*
 * A292874  Least k such that A039654(k) = prime(n).
 * https://oeis.org/A292874
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k such that A039654(k) = prime(n); found by search
 *            over k = 2..prime(n), iterating x -> sigma(x)-1 until a prime is
 *            reached
 * Range:     u8 n <= 54, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Validated: partial, pass, 57 terms through n = 57 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static bool A292874_isp(uint64_t m)
{
    if (m < 2) return false;
    for (uint64_t d = 2; d * d <= m; ++d)
        if (m % d == 0) return false;
    return true;
}

static uint64_t A292874_sigma(uint64_t m)
{
    uint64_t s = 0;
    for (uint64_t d = 1; d * d <= m; ++d) {
        if (m % d == 0) {
            s += d;
            uint64_t e = m / d;
            if (e != d) s += e;
        }
    }
    return s;
}

/* returns prime reached from k, or 0 if none */
static uint64_t A292874_reach(uint64_t k)
{
    uint64_t cur = k;
    for (int it = 0; it < 1000; ++it) {
        if (A292874_isp(cur)) return cur;
        if (cur > 1000000000000ULL) return 0;
        uint64_t nx = A292874_sigma(cur) - 1;
        if (nx == cur) return 0;
        cur = nx;
    }
    return 0;
}

seq_t a(idx_t n)
{
    uint64_t cnt = 0, p = 1;
    while (cnt < (uint64_t)n) {
        ++p;
        if (A292874_isp(p)) ++cnt;
    }
    for (uint64_t k = 2; k <= p; ++k) {
        if (A292874_reach(k) == p) return (seq_t)k;
    }
    return (seq_t)p;
}
