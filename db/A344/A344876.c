/*
 * A344876  a(n) = A344875(n) - A011772(n).
 * https://oeis.org/A344876
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = A344875(n) - A011772(n), where A344875 is multiplicative
 *            with A344875(2^e) = 2^(e+1)-1, A344875(p^e) = p^e-1 for odd p,
 *            and A011772(n) is the least m with n | m(m+1)/2 (found by
 *            search)
 * Range:     u8 n <= 175, u16 n <= 16384, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 108 terms through n = 9634 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A344875_a(idx_t n) {
    seq_t ans = 1;
    uint64_t tmp = (uint64_t)n;
    uint64_t d = 2;
    while (tmp > 1) {
        uint64_t p = 0, e = 0;
        if (tmp % 2 == 0) {
            p = 2;
        } else {
            for (d = 3; d * d <= tmp; d += 2) {
                if (tmp % d == 0) { p = d; break; }
            }
            if (p == 0) p = tmp;
        }
        while (tmp % p == 0) {
            tmp /= p;
            ++e;
        }
        seq_t term;
        if (p == 2) {
            seq_t pw = 1;
            for (uint64_t i = 0; i < e + 1; ++i) pw *= 2;
            term = pw - 1;
        } else {
            seq_t pw = 1;
            for (uint64_t i = 0; i < e; ++i) pw *= (seq_t)p;
            term = pw - 1;
        }
        ans *= term;
    }
    return ans;
}

static uint64_t A011772_a(idx_t n) {
    if (n == 0) return 0;
    uint64_t limit = (uint64_t)n * 2u;
    for (uint64_t m = 1; m < limit; ++m) {
        unsigned __int128 t = (unsigned __int128)m * (m + 1) / 2;
        if (t % n == 0) return m;
    }
    return limit - 1;
}

seq_t a(idx_t n) {
    return A344875_a(n) - (seq_t)A011772_a(n);
}
