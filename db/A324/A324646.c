/*
 * A324646  a(n) = gcd(n, A276086(n-1)).
 * https://oeis.org/A324646
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(n, A276086(n-1)), computed as prod_p p^min(v_p(n),
 *            d_k), where d_k are the primorial-base digits of n-1 and p =
 *            prime(k+1)
 * Range:     u8 n <= 293, u16 n <= 30030, u32 n <= 30030, u64 n <= 30030,
 *            u128 n <= 30030, big any n
 * Validated: standard, pass, 121 terms through n = 8924 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static uint64_t A324646_next_prime(uint64_t p)
{
    uint64_t c = p + 1;
    for (;; c++) {
        bool ok = true;
        for (uint64_t d = 2; d * d <= c; d++) {
            if (c % d == 0) { ok = false; break; }
        }
        if (ok && c >= 2) return c;
    }
}

seq_t a(idx_t n)
{
    uint64_t nn = (uint64_t)n;
    uint64_t m = nn - 1;
    uint64_t p = 2;
    uint64_t res = 1;
    while (m > 0) {
        uint64_t d = m % p;
        m /= p;
        if (d > 0) {
            uint64_t t = nn;
            uint64_t cnt = 0;
            while (cnt < d && t % p == 0) {
                t /= p;
                res *= p;
                cnt++;
            }
        }
        p = A324646_next_prime(p);
    }
    return (seq_t)res;
}
