/*
 * A308267  Numbers k such that k divides A048678(k).
 * https://oeis.org/A308267
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n-th k >= 1 such that k | A048678(k); found by search,
 *            computing A048678(k) mod k via A048678(2m)=2*A048678(m),
 *            A048678(2m+1)=4*A048678(m)+1
 * Range:     u8 n <= 22, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Validated: partial, pass, 55 terms through n = 55 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static int A308267_ok(uint64_t k)
{
    int msb = 0;
    uint64_t t = k;
    while (t) { msb++; t >>= 1; }
    uint64_t r = 0;
    for (int i = msb - 1; i >= 0; i--) {
        if ((k >> i) & 1u) r = (r * 4 + 1) % k;
        else r = (r * 2) % k;
    }
    return r == 0;
}

seq_t a(idx_t n)
{
    uint64_t k = 0;
    idx_t cnt = 0;
    while (cnt < n) {
        k++;
        if (A308267_ok(k)) cnt++;
    }
    return (seq_t)k;
}
