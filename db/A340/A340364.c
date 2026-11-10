/*
 * A340364  a(n) = gcd(A005940(n), A324106(n)), where A324106(n) is
 *          multiplicative with a(p^e) = A005940(p^e).
 * https://oeis.org/A340364
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(A005940(n), A324106(n)), where A324106 is
 *            multiplicative with A324106(p^e) = A005940(p^e)
 * Range:     u8 n <= 56, u16 n <= 508, u32 n <= 16272, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 99 terms through n = 9843 (2026-11-10)
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"

static seq_t A340364_doudna(idx_t n) {
    static uint64_t pr[80];
    static int init = 0;
    if (!init) {
        int c = 0;
        uint64_t x = 2;
        while (c < 80) {
            bool ok = true;
            for (uint64_t d = 2; d * d <= x; d++) {
                if (x % d == 0) { ok = false; break; }
            }
            if (ok) pr[c++] = x;
            x++;
        }
        init = 1;
    }
    idx_t m = n - 1;
    seq_t result = 1;
    int i = 1;
    while (m > 0) {
        if (m & 1) {
            result *= (seq_t)pr[i - 1];
            m = (m - 1) / 2;
        } else {
            m /= 2;
            i++;
        }
    }
    return result;
}

seq_t a(idx_t n) {
    seq_t d = A340364_doudna(n);
    seq_t mult = 1;
    idx_t m = n;
    for (idx_t p = 2; p * p <= m; p++) {
        if (m % p == 0) {
            idx_t q = 1;
            while (m % p == 0) { m /= p; q *= p; }
            mult *= A340364_doudna(q);
        }
    }
    if (m > 1) mult *= A340364_doudna(m);
    return gcd(d, mult);
}
