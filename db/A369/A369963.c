/*
 * A369963  Numbers k for which x = gcd(k, A003415(k)) = gcd(k, A276086(k))
 *          and x > 1, where A003415 is the arithmetic derivative, and A276086
 *          is the primorial base exp-function.
 * https://oeis.org/A369963
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search: k is a term iff A085731(k) == A324198(k) and A085731(k)
 *            > 1, where A085731(k) = gcd(k, A003415(k)) = prod_{p^e||k} p^(e
 *            if p|e else e-1) and A324198(k) = gcd(k, A276086(k)) =
 *            prod_{p^e||k} p^min(e, d_p(k)), d_p(k) being the primorial-base
 *            digit of k belonging to prime p
 * Range:     u8 n <= 9, u16 n <= 1707, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 75 terms through n = 8332 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

static uint64_t *A369963_terms = NULL;
static size_t A369963_cnt = 0;
static size_t A369963_cap = 0;
static uint64_t A369963_next = 2;

static bool A369963_test(uint64_t k)
{
    static const uint64_t pr[15] = {2,3,5,7,11,13,17,19,23,29,31,37,41,43,47};
    uint64_t dig[15];
    uint64_t P = 1;
    for (int i = 0; i < 15; ++i) {
        dig[i] = (k / P) % pr[i];
        P *= pr[i];
    }
    uint64_t g1 = 1, g2 = 1;
    uint64_t m = k;
    for (uint64_t p = 2; p * p <= m; p += (p == 2 ? 1 : 2)) {
        if (m % p == 0) {
            uint64_t e = 0;
            while (m % p == 0) { m /= p; ++e; }
            uint64_t e1 = (e % p == 0) ? e : e - 1;
            uint64_t e2 = 0;
            for (int i = 0; i < 15; ++i) {
                if (pr[i] == p) { e2 = dig[i] < e ? dig[i] : e; break; }
            }
            for (uint64_t t = 0; t < e1; ++t) g1 *= p;
            for (uint64_t t = 0; t < e2; ++t) g2 *= p;
        }
    }
    if (m > 1) {
        /* prime with exponent 1: contributes p^0 to g1 */
        for (int i = 0; i < 15; ++i) {
            if (pr[i] == m) { if (dig[i] >= 1) g2 *= m; break; }
        }
    }
    return g1 == g2 && g1 > 1;
}

seq_t a(idx_t n)
{
    while (A369963_cnt < (size_t)n) {
        uint64_t k = A369963_next++;
        if (A369963_test(k)) {
            if (A369963_cnt == A369963_cap) {
                A369963_cap = A369963_cap ? A369963_cap * 2 : 256;
                A369963_terms = (uint64_t *)realloc(A369963_terms, A369963_cap * sizeof(uint64_t));
                if (!A369963_terms) abort();
            }
            A369963_terms[A369963_cnt++] = k;
        }
    }
    return (seq_t)A369963_terms[(size_t)n - 1];
}
