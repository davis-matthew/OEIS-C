/*
 * A364500  a(n) = gcd(n, A005940(n)).
 * https://oeis.org/A364500
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(n, A005940(n)); A005940(n) = prod prime(i)^e_i, where
 *            e_i = number of 1's in binary(n-1) followed by exactly i-1 0's;
 *            a(n) = prod p^min(v_p(n), e_p)
 * Range:     u8 n <= 255, u16 n <= 16384, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 116 terms through n = 7728 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    int e[72];
    int k;
    for (k = 0; k < 72; k++) e[k] = 0;
    idx_t m = n - 1;
    int i = 1;
    while (m > 0) {
        if (m & 1) {
            e[i]++;
            m = (m - 1) / 2;
        } else {
            m /= 2;
            i++;
        }
    }
    idx_t res = 1;
    uint64_t p = 1;
    for (k = 1; k <= i && k < 72; k++) {
        /* next prime after p */
        for (;;) {
            uint64_t d;
            int ok = 1;
            p++;
            for (d = 2; d * d <= p; d++) {
                if (p % d == 0) { ok = 0; break; }
            }
            if (ok) break;
        }
        if (e[k] > 0) {
            int c = 0;
            idx_t t = n;
            while (c < e[k] && t % p == 0) {
                t /= p;
                res *= p;
                c++;
            }
        }
    }
    return (seq_t)res;
}
