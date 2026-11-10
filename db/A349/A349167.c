/*
 * A349167  a(n) = 1 if sigma(n) and A003961(n) are relatively prime,
 *          otherwise 0.
 * https://oeis.org/A349167
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if gcd(sigma(n), A003961(n)) = 1, else 0; A003961 has
 *            prime factors next_prime(p) for each p | n, so test whether any
 *            such prime divides a sigma(p^e) factor
 * Range:     u8 n <= 65537, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 136 terms through n = 8306 (2026-11-10)
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/next_prime.h"

seq_t a(idx_t n) {
    uint64_t m = (uint64_t)n;
    uint64_t ps[24];
    uint64_t ss[24];
    int cnt = 0;
    for (uint64_t p = 2; p * p <= m; p++) {
        if (m % p == 0) {
            uint64_t s = 1, pw = 1;
            while (m % p == 0) {
                m /= p;
                pw *= p;
                s += pw;
            }
            ps[cnt] = p;
            ss[cnt] = s;
            cnt++;
        }
    }
    if (m > 1) {
        ps[cnt] = m;
        ss[cnt] = m + 1;
        cnt++;
    }
    for (int i = 0; i < cnt; i++) {
        uint64_t q = next_prime(ps[i]);
        for (int j = 0; j < cnt; j++) {
            if (ss[j] % q == 0) return (seq_t)0;
        }
    }
    return (seq_t)1;
}
