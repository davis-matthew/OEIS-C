/*
 * A344973  a(n) = A344875(n) mod A011772(n).
 * https://oeis.org/A344973
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A344875(n) mod A011772(n), where A344875 is
 *            multiplicative with a(2^e) = 2^(e+1) - 1 and a(p^e) = p^e - 1
 *            for odd p, and A011772(n) is the least m >= 1 with n | m(m+1)/2
 *            (found by search)
 * Range:     u8 n <= 177, u16 n <= 11264, u32 n <= 11264, u64 n <= 11264,
 *            u128 n <= 11264, big any n
 * Validated: standard, pass, 115 terms through n = 9870 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A344973_A344875(idx_t n) {
    seq_t ans = 1;
    uint64_t tmp = (uint64_t)n;
    uint64_t p = 2;
    while (tmp > 1) {
        if (p * p > tmp) p = tmp;
        if (tmp % p == 0) {
            unsigned e = 0;
            while (tmp % p == 0) { tmp /= p; ++e; }
            seq_t pw = 1;
            if (p == 2) {
                for (unsigned i = 0; i < e + 1; ++i) pw *= 2;
            } else {
                for (unsigned i = 0; i < e; ++i) pw *= (seq_t)p;
            }
            ans *= (pw - 1);
        }
        p += (p == 2) ? 1 : 2;
    }
    return ans;
}

static idx_t A344973_A011772(idx_t n) {
    if (n == 0) return 0;
    idx_t limit = 2 * n;
    for (idx_t m = 1; m < limit; ++m) {
        unsigned __int128 t = (unsigned __int128)m * (m + 1) / 2;
        if (t % n == 0) return m;
    }
    return limit - 1;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;
    seq_t x = A344973_A344875(n);
    idx_t m = A344973_A011772(n);
    return x % (seq_t)m;
}
