/*
 * A280262  Numbers n such that A187730(n) < A049559(n).
 * https://oeis.org/A280262
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k with gcd(lambda(k),k-1) < gcd(phi(k),k-1); found
 *            by incremental search over k, cached
 * Range:     u8 n <= 25, u16 n <= 8560, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 77 terms through n = 7829 (2026-11-08)
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

static uint64_t A280262_gcd(uint64_t a, uint64_t b)
{
    while (b) { uint64_t t = a % b; a = b; b = t; }
    return a;
}

static int A280262_test(uint64_t m)
{
    if (m < 3) return 0;
    uint64_t pr[64], ex[64];
    size_t cnt = 0;
    factor_u64(m, pr, ex, &cnt);
    uint64_t lam = 1, phi = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = pr[i], e = ex[i];
        uint64_t pp = 1;
        for (uint64_t j = 1; j < e; ++j) pp *= p;   /* p^(e-1) */
        uint64_t ph = pp * (p - 1);
        uint64_t lp = ph;
        if (p == 2 && e >= 3) lp = ph / 2;
        phi *= ph;
        uint64_t g = A280262_gcd(lam, lp);
        lam = (lam / g) * lp;
    }
    uint64_t m1 = m - 1;
    return A280262_gcd(lam, m1) < A280262_gcd(phi, m1);
}

seq_t a(idx_t n)
{
    static uint64_t *terms = NULL;
    static size_t count = 0, cap = 0;
    static uint64_t next = 3;
    if (n < 1) return 0;
    while (count < (size_t)n) {
        if (A280262_test(next)) {
            if (count == cap) {
                cap = cap ? cap * 2 : 1024;
                terms = (uint64_t *)realloc(terms, cap * sizeof(uint64_t));
            }
            terms[count++] = next;
        }
        ++next;
    }
    return (seq_t)terms[n - 1];
}
