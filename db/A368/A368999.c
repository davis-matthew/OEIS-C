/*
 * A368999  Numbers k for which A342001(k) is odd.
 * https://oeis.org/A368999
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = n-th k with A342001(k) odd. A342001(k) is odd iff (k even
 *            and v2(k) odd) or (k odd and bigomega(k) odd); found by scanning
 *            k upward
 * Range:     u8 n <= 157, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 94 terms through n = 9731 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static int A368999_odd(idx_t k)
{
    if (k <= 1) return 0;
    if ((k & 1) == 0) {
        int v = 0;
        while ((k & 1) == 0) { k >>= 1; ++v; }
        return v & 1;
    }
    int cnt = 0;
    for (idx_t p = 3; p <= k / p; p += 2) {
        while (k % p == 0) { k /= p; ++cnt; }
    }
    if (k > 1) ++cnt;
    return cnt & 1;
}

seq_t a(idx_t n)
{
    idx_t k = 1;
    idx_t c = 0;
    while (1) {
        if (A368999_odd(k)) {
            ++c;
            if (c == n) return (seq_t)k;
        }
        ++k;
    }
}
