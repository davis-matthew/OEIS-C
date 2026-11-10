/*
 * A337610  Positive integers m such that A126287^k(m) = m for some positive
 *          integer k.
 * https://oeis.org/A337610
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n-th positive m with A126287(A126287(m)) = m, where
 *            A126287(1)=A126287(2)=1, A126287(k) = k*lpf(k-1)/lpf(k); found
 *            by a search
 * Range:     u8 n <= 46, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 72 terms through n = 8782 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static uint64_t lpf_A337610(uint64_t n)
{
    if (n <= 1) return 1;
    if ((n & 1) == 0) return 2;
    for (uint64_t d = 3; d * d <= n; d += 2)
        if (n % d == 0) return d;
    return n;
}

static uint64_t A126287_a(uint64_t n)
{
    if (n <= 2) return 1;
    return n * lpf_A337610(n - 1) / lpf_A337610(n);
}

seq_t a(idx_t n)
{
    static uint64_t last_n = 0, last_m = 0;
    uint64_t cnt, m;
    if (last_n != 0 && (uint64_t)n >= last_n) {
        cnt = last_n;
        m = last_m;
    } else {
        cnt = 0;
        m = 0;
    }
    while (cnt < (uint64_t)n) {
        m++;
        if (A126287_a(A126287_a(m)) == m) cnt++;
    }
    last_n = (uint64_t)n;
    last_m = m;
    return (seq_t)m;
}
