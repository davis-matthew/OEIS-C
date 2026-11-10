/*
 * A345948  a(n) = A344875(n) / gcd(A153151(n), A344875(n)).
 * https://oeis.org/A345948
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A344875(n) / gcd(A153151(n), A344875(n)); A344875
 *            multiplicative with a(2^e)=2^(e+1)-1, a(p^e)=p^e-1 for odd p;
 *            A153151(n)=2n-1 if n is a power of 2 (n>=2), n-1 otherwise,
 *            a(1)=1
 * Range:     u8 n <= 151, u16 n <= 16384, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 108 terms through n = 9932 (2026-11-10)
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

static seq_t A344875_a(idx_t n) {
    seq_t ans = 1;
    uint64_t tmp = (uint64_t)n;
    uint64_t p;
    for (p = 2; p * p <= tmp; p += (p == 2 ? 1 : 2)) {
        if (tmp % p == 0) {
            seq_t pw = 1;
            while (tmp % p == 0) {
                tmp /= p;
                pw *= (seq_t)p;
            }
            if (p == 2) pw *= 2;
            ans *= (pw - 1);
        }
    }
    if (tmp > 1) {
        ans *= ((seq_t)tmp - 1);
    }
    return ans;
}

static seq_t A153151_a(idx_t n) {
    if (n < 2) return (seq_t)n;
    if ((n & (n - 1)) == 0) return (seq_t)n * 2 - 1;
    return (seq_t)n - 1;
}

seq_t a(idx_t n) {
    seq_t x = A344875_a(n);
    seq_t y = A153151_a(n);
    seq_t g = gcd(y, x);
    return x / g;
}
