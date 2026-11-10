/*
 * A346046  Decimal expansion of Sum_{k>=1} 1/A053657(k).
 * https://oeis.org/A346046
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = n-th decimal digit of Sum_{k>=1} 1/A053657(k), where
 *            A053657(k)/A053657(k-1) = Product_{p prime, j>=0, (p-1)p^j |
 *            (k-1)} p. Computed with a base-1e9 fixed-point big number (terms
 *            obtained by repeated small division)
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Validated: partial, pass, 100 terms through n = 100 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static bool A346046_isprime(uint64_t x) {
    if (x < 2) return false;
    if (x < 4) return true;
    if (x % 2 == 0) return false;
    for (uint64_t d = 3; d * d <= x; d += 2)
        if (x % d == 0) return false;
    return true;
}

seq_t a(idx_t n)
{
    size_t L = (size_t)((n - 1 + 8) / 9) + 4;   /* fractional limbs */
    size_t sz = L + 1;
    const uint64_t BASE = 1000000000ULL;
    uint32_t *S = (uint32_t *)calloc(sz, sizeof(uint32_t));
    uint32_t *t = (uint32_t *)calloc(sz, sizeof(uint32_t));
    if (!S || !t) { free(S); free(t); return (seq_t)0; }
    t[0] = 1;
    size_t nz = 0;
    for (uint64_t k = 1;; k++) {
        if (k >= 2) {
            uint64_t m = k - 1;
            for (uint64_t d = 1; d <= m; d++) {
                if (m % d) continue;
                uint64_t p = d + 1;
                if (!A346046_isprime(p)) continue;
                uint64_t q = d;
                while (m % q == 0) {
                    /* divide t by p */
                    uint64_t rem = 0;
                    for (size_t i = nz; i < sz; i++) {
                        uint64_t cur = rem * BASE + t[i];
                        t[i] = (uint32_t)(cur / p);
                        rem = cur % p;
                    }
                    while (nz < sz && t[nz] == 0) nz++;
                    if (nz >= sz) break;
                    if (q > m / p) break;
                    q *= p;
                }
                if (nz >= sz) break;
            }
        }
        if (nz >= sz) break;
        uint64_t carry = 0;
        for (size_t i = sz; i-- > nz;) {
            uint64_t cur = (uint64_t)S[i] + t[i] + carry;
            if (cur >= BASE) { cur -= BASE; carry = 1; } else carry = 0;
            S[i] = (uint32_t)cur;
        }
        for (size_t i = nz; carry && i-- > 0;) {
            uint64_t cur = (uint64_t)S[i] + carry;
            if (cur >= BASE) { cur -= BASE; carry = 1; } else carry = 0;
            S[i] = (uint32_t)cur;
        }
    }
    int digit;
    if (n == 1) {
        digit = (int)S[0];
    } else {
        uint64_t f = (uint64_t)n - 1;
        size_t li = 1 + (size_t)((f - 1) / 9);
        int pos = (int)((f - 1) % 9);
        uint32_t v = S[li];
        for (int i = 0; i < 8 - pos; i++) v /= 10;
        digit = (int)(v % 10);
    }
    free(S);
    free(t);
    return (seq_t)digit;
}
