/*
 * A397547  a(n) = numerator of A069835(n)/2^n.
 * https://oeis.org/A397547
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  frac, nonn
 * Formula:   a(n) = numerator of A069835(n)/2^n, where A069835(n) =
 *            Sum_{k=0..n} 3^k * binomial(n,k)^2; i.e. a(n) = A069835(n) /
 *            2^min(n, v2(A069835(n)))
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 23, u128 n <= 47,
 *            big any n
 * Validated: standard, pass, 39 terms through n = 97 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static void A397547_mul(uint32_t *x, size_t L, uint32_t m)
{
    uint64_t c = 0;
    for (size_t i = 0; i < L; ++i)
    {
        uint64_t v = (uint64_t)x[i] * m + c;
        x[i] = (uint32_t)v;
        c = v >> 32;
    }
}

static void A397547_div(uint32_t *x, size_t L, uint32_t d)
{
    uint64_t r = 0;
    for (size_t i = L; i-- > 0;)
    {
        uint64_t v = (r << 32) | x[i];
        x[i] = (uint32_t)(v / d);
        r = v % d;
    }
}

static void A397547_add(uint32_t *s, const uint32_t *x, size_t L)
{
    uint64_t c = 0;
    for (size_t i = 0; i < L; ++i)
    {
        uint64_t v = (uint64_t)s[i] + x[i] + c;
        s[i] = (uint32_t)v;
        c = v >> 32;
    }
}

seq_t a(idx_t n)
{
    size_t L = (size_t)(n * 4 + 64) / 32 + 2;
    uint32_t *t = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *s = (uint32_t *)calloc(L, sizeof(uint32_t));
    t[0] = 1;
    for (idx_t k = 0; k <= n; ++k)
    {
        A397547_add(s, t, L);
        if (k == n) break;
        uint32_t f = (uint32_t)(n - k);
        A397547_mul(t, L, f);
        A397547_mul(t, L, f);
        A397547_mul(t, L, 3);
        A397547_div(t, L, (uint32_t)(k + 1));
        A397547_div(t, L, (uint32_t)(k + 1));
    }

    size_t total = L * 32;
    size_t top = 0;
    for (size_t i = total; i-- > 0;)
    {
        if ((s[i / 32] >> (i % 32)) & 1u) { top = i; break; }
    }
    size_t lo = 0;
    while ((idx_t)lo < n && lo < top && !((s[lo / 32] >> (lo % 32)) & 1u)) ++lo;

    seq_t r = 0;
    for (size_t i = top + 1; i-- > lo;)
    {
        r = r + r;
        if ((s[i / 32] >> (i % 32)) & 1u) r = r + 1;
    }
    free(t);
    free(s);
    return r;
}
