/*
 * A351555  a(n) is the number of odd prime factors of sigma(n) that do not
 *          divide A003961(n) and the valuation(n, p) is different from
 *          valuation(sigma(n), p).
 * https://oeis.org/A351555
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of odd primes p | sigma(n) with p not dividing
 *            A003961(n) and valuation(n,p) != valuation(sigma(n),p). Computed
 *            by trial-division factorization
 * Range:     u8 n <= 65537, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 9937 (2026-11-10)
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/next_prime.h"

seq_t a(idx_t n) {
    uint64_t m = (uint64_t)n;
    uint64_t np[20];   /* primes dividing A003961(n) */
    int nnp = 0;
    uint64_t sig = 1;
    for (uint64_t p = 2; p * p <= m; p++) {
        if (m % p == 0) {
            uint64_t pe = 1, s = 1;
            while (m % p == 0) {
                m /= p;
                pe *= p;
                s += pe;
            }
            sig *= s;
            np[nnp++] = next_prime(p);
        }
    }
    if (m > 1) {
        sig *= (m + 1);
        np[nnp++] = next_prime(m);
    }
    uint64_t s = sig;
    uint64_t cnt = 0;
    for (uint64_t p = 2; p * p <= s || s > 1; p++) {
        if (p * p > s) p = s;
        if (s % p != 0) continue;
        uint64_t vs = 0;
        while (s % p == 0) { s /= p; vs++; }
        if (p == 2) continue;
        bool dv = false;
        for (int i = 0; i < nnp; i++) if (np[i] == p) dv = true;
        if (dv) continue;
        uint64_t vn = 0, t = (uint64_t)n;
        while (t % p == 0) { t /= p; vn++; }
        if (vn != vs) cnt++;
    }
    return (seq_t)cnt;
}
