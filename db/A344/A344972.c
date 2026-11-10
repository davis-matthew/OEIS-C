/*
 * A344972  a(n) = floor(A344875(n) / A011772(n)).
 * https://oeis.org/A344972
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(A344875(n) / A011772(n)); A344875 is multiplicative
 *            with f(2^e)=2^(e+1)-1 and f(p^e)=p^e-1 for odd p; A011772(n) is
 *            found by search (smallest m with n | m(m+1)/2)
 * Range:     u8 n <= 1963, u16 n <= 9977, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 9977 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A344972_A344875(idx_t n) {
    seq_t ans = 1;
    uint64_t tmp = (uint64_t)n;
    uint64_t p = 2;
    while (tmp > 1) {
        if (p * p > tmp) p = tmp;
        if (tmp % p == 0) {
            seq_t pw = 1;
            while (tmp % p == 0) {
                tmp /= p;
                pw *= (seq_t)p;
            }
            if (p == 2) pw *= 2;
            ans *= (pw - 1);
        }
        if (p == 2) p = 3; else p += 2;
    }
    return ans;
}

static idx_t A344972_A011772(idx_t n) {
    idx_t limit = n * 2;
    for (idx_t m = 1; m < limit; ++m) {
        unsigned __int128 t = ((unsigned __int128)m * ((unsigned __int128)m + 1)) / 2;
        if (t % n == 0) return m;
    }
    return limit - 1;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;
    seq_t num = A344972_A344875(n);
    idx_t d = A344972_A011772(n);
    return num / (seq_t)d;
}
