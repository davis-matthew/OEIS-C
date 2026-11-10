/*
 * A346404  Decimal expansion of Sum_{k>=1} A003418(k)/k!.
 * https://oeis.org/A346404
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = n-th digit of Sum_{k>=1} A003418(k)/k!, with
 *            A003418(k)=lcm(1..k); terms t_k = t_{k-1}*r/k, r=p if k=p^m else
 *            1, in base-1e9 fixed point
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Validated: partial, pass, 100 terms through n = 100 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n) {
    size_t F = (size_t)((n + 40) / 9 + 2);
    size_t L = F + 1;
    uint32_t *t = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *s = (uint32_t *)calloc(L, sizeof(uint32_t));
    const uint64_t BASE = 1000000000ULL;
    t[0] = 1;
    s[0] = 1;
    for (uint64_t k = 2;; k++) {
        /* r = p if k is a prime power p^m, else 1 */
        uint64_t r = 1, m = k, p = 0;
        for (uint64_t d = 2; d * d <= m; d++) {
            if (m % d == 0) { p = d; break; }
        }
        if (p == 0) p = k;
        while (m % p == 0) m /= p;
        if (m == 1) r = p;
        /* t = t * r / k */
        if (r > 1) {
            uint64_t carry = 0;
            for (size_t i = L; i-- > 0;) {
                uint64_t v = (uint64_t)t[i] * r + carry;
                t[i] = (uint32_t)(v % BASE);
                carry = v / BASE;
            }
        }
        uint64_t rem = 0;
        int nonzero = 0;
        for (size_t i = 0; i < L; i++) {
            uint64_t v = rem * BASE + t[i];
            t[i] = (uint32_t)(v / k);
            rem = v % k;
            if (t[i]) nonzero = 1;
        }
        if (!nonzero) break;
        uint64_t c = 0;
        for (size_t i = L; i-- > 0;) {
            uint64_t v = (uint64_t)s[i] + t[i] + c;
            s[i] = (uint32_t)(v % BASE);
            c = v / BASE;
        }
    }
    unsigned digit;
    if (n == 1) {
        digit = s[0] % 10;
    } else {
        uint64_t j = (uint64_t)n - 1;
        size_t limb = 1 + (size_t)((j - 1) / 9);
        unsigned pos = (unsigned)((j - 1) % 9);
        uint32_t v = s[limb];
        for (unsigned i = 0; i < 8 - pos; i++) v /= 10;
        digit = v % 10;
    }
    free(t);
    free(s);
    return (seq_t)digit;
}
