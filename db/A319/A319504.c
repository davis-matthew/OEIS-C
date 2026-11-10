/*
 * A319504  Indices of valleys (i.e. local minima) in A064800.
 * https://oeis.org/A319504
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = index k of the n-th valley of A064800(k) = k +
 *            bigomega(k); found by scanning k: bigomega(k-1) >= bigomega(k)+2
 *            and bigomega(k+1) >= bigomega(k)
 * Range:     u8 n <= 67, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Validated: partial, pass, 78 terms through n = 78 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static unsigned A319504_omega(uint64_t t)
{
    unsigned c = 0;
    while ((t & 1ULL) == 0) { ++c; t >>= 1; }
    for (uint64_t p = 3; p <= t / p; p += 2) {
        while (t % p == 0) { ++c; t /= p; }
    }
    if (t > 1) ++c;
    return c;
}

seq_t a(idx_t n)
{
    uint64_t cnt = 0;
    uint64_t k = 2;
    unsigned prev = A319504_omega(1);
    unsigned cur = A319504_omega(2);
    for (;; ++k) {
        unsigned next = A319504_omega(k + 1);
        if (prev >= cur + 2 && next >= cur) {
            ++cnt;
            if (cnt == (uint64_t)n) return (seq_t)k;
        }
        prev = cur;
        cur = next;
    }
}
