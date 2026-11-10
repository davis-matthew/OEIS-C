/*
 * A258619  Decimal expansion of Sum_{k>=0} (1/A055209(k)).
 * https://oeis.org/A258619
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = n-th decimal digit of Sum_{k>=0} 1/A055209(k), where
 *            A055209(k) = Prod_{i=0..k} (i!)^2; computed with fixed-point
 *            base-1e9 arithmetic, dividing by j twice for j=1..k to get
 *            1/A055209(k) from 1/A055209(k-1)
 * Range:     u8 n <= 5000, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Validated: standard, pass, 121 terms through n = 4953 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

static unsigned char *A258619_digits = NULL;
static size_t A258619_count = 0;

static void A258619_divide(uint32_t *t, size_t len, uint32_t d)
{
    uint64_t rem = 0;
    for (size_t i = 0; i < len; i++) {
        uint64_t cur = rem * 1000000000ULL + t[i];
        t[i] = (uint32_t)(cur / d);
        rem = cur % d;
    }
}

static void A258619_compute(size_t N)
{
    size_t L = (N + 40) / 9 + 3; /* limbs: 1 integer + fractional */
    uint32_t *t = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *s = (uint32_t *)calloc(L, sizeof(uint32_t));
    unsigned char *dg = (unsigned char *)malloc(N + 1);
    if (!t || !s || !dg) { free(t); free(s); free(dg); return; }
    t[0] = 1;
    for (uint32_t k = 0; ; k++) {
        for (uint32_t j = 1; j <= k; j++) {
            A258619_divide(t, L, j);
            A258619_divide(t, L, j);
        }
        int nz = 0;
        for (size_t i = 0; i < L; i++) if (t[i]) { nz = 1; break; }
        if (!nz) break;
        uint32_t carry = 0;
        for (size_t i = L; i-- > 0; ) {
            uint32_t v = s[i] + t[i] + carry;
            if (v >= 1000000000U) { v -= 1000000000U; carry = 1; } else carry = 0;
            s[i] = v;
        }
    }
    dg[0] = (unsigned char)(s[0] % 10);
    for (size_t m = 1; m < N; m++) {
        size_t limb = 1 + (m - 1) / 9;
        unsigned p = (unsigned)((m - 1) % 9);
        uint32_t div = 1;
        for (unsigned q = 0; q < 8 - p; q++) div *= 10;
        dg[m] = (unsigned char)((s[limb] / div) % 10);
    }
    free(t);
    free(s);
    free(A258619_digits);
    A258619_digits = dg;
    A258619_count = N;
}

seq_t a(idx_t n)
{
    size_t need = (size_t)n;
    if (need < 1) return 0;
    if (need > A258619_count) {
        size_t N = need;
        if (N < 64) N = 64;
        if (N < A258619_count * 2) N = A258619_count * 2;
        A258619_compute(N);
        if (need > A258619_count) return 0;
    }
    return (seq_t)A258619_digits[need - 1];
}
