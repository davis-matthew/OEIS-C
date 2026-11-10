/*
 * A348135  Irregular triangle T(n, k), n > 0, k = 1..A067399(n), read by
 *          rows; the n-th row gives, in ascending order, the distinct
 *          integers k such that A067138(k, m) = n for some m.
 * https://oeis.org/A348135
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   flattened rows of T(n,k); row n lists, ascending, the OR-numbral
 *            divisors d of n (d such that n = OR of (d<<i) over bits i of
 *            some e). Computed by search
 * Range:     u8 n <= 1563, u16 n <= 7412, u32 n <= 7412, u64 n <= 7412,
 *            u128 n <= 7412, big any n
 * Validated: standard, pass, 100 terms through n = 7325 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static int A348135_isdiv(uint64_t u, uint64_t d) {
    int msbn = 63 - __builtin_clzll(u);
    int msbd = 63 - __builtin_clzll(d);
    uint64_t mask = 0;
    for (int s = 0; s <= msbn - msbd; ++s) {
        uint64_t sh = d << s;
        if ((sh & ~u) == 0) mask |= sh;
    }
    return mask == u;
}

seq_t a(idx_t n) {
    uint64_t pos = (uint64_t)n;
    uint64_t r = 1;
    if (pos == 0) return 0;
    for (;; ++r) {
        uint64_t cnt = 0;
        for (uint64_t d = 1; d <= r; ++d)
            if (A348135_isdiv(r, d)) ++cnt;
        if (pos <= cnt) break;
        pos -= cnt;
    }
    for (uint64_t d = 1; d <= r; ++d) {
        if (A348135_isdiv(r, d)) {
            if (--pos == 0) return (seq_t)d;
        }
    }
    return 0;
}
