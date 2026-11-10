/*
 * A390243  Decimal expansion of Sum_{k>=0} (-1)^k/Catalan(k).
 * https://oeis.org/A390243
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = n-th digit after the decimal point of Sum_{k>=0}
 *            (-1)^k/Catalan(k), with terms t(k) = t(k-1)*(k+1)/(2*(2k-1)),
 *            t(0) = 1, summed in fixed-point base 10^9
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Validated: partial, pass, 105 terms through n = 104 (2026-11-03)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define C390243_BASE 1000000000u

static uint8_t *c390243_digits = NULL;
static size_t c390243_have = 0;

static void c390243_add(uint32_t *s, const uint32_t *t, size_t lead, size_t len) {
    uint64_t carry = 0;
    size_t i;
    for (i = len; i > lead; i--) {
        uint64_t v = (uint64_t)s[i - 1] + t[i - 1] + carry;
        if (v >= C390243_BASE) { v -= C390243_BASE; carry = 1; } else carry = 0;
        s[i - 1] = (uint32_t)v;
    }
    i = lead;
    while (carry && i > 0) {
        uint64_t v = (uint64_t)s[i - 1] + carry;
        if (v >= C390243_BASE) { v -= C390243_BASE; carry = 1; } else carry = 0;
        s[i - 1] = (uint32_t)v;
        i--;
    }
}

static void c390243_compute(size_t D) {
    size_t F = (D + 30) / 9 + 2;
    size_t len = F + 1;
    uint32_t *t = (uint32_t *)calloc(len, sizeof(uint32_t));
    uint32_t *pos = (uint32_t *)calloc(len, sizeof(uint32_t));
    uint32_t *neg = (uint32_t *)calloc(len, sizeof(uint32_t));
    uint8_t *out = (uint8_t *)malloc(D);
    size_t lead = 0, i;
    uint64_t k;

    t[0] = 1;
    pos[0] = 1;
    for (k = 1; ; k++) {
        uint64_t m = k + 1, d = 2 * (2 * k - 1);
        uint64_t carry = 0, rem = 0;
        for (i = len; i > lead; i--) {
            uint64_t v = (uint64_t)t[i - 1] * m + carry;
            t[i - 1] = (uint32_t)(v % C390243_BASE);
            carry = v / C390243_BASE;
        }
        while (carry && lead > 0) {
            uint64_t v = (uint64_t)t[lead - 1] + carry;
            t[lead - 1] = (uint32_t)(v % C390243_BASE);
            carry = v / C390243_BASE;
            lead--;
        }
        for (i = lead; i < len; i++) {
            uint64_t v = rem * C390243_BASE + t[i];
            t[i] = (uint32_t)(v / d);
            rem = v % d;
        }
        while (lead < len && t[lead] == 0) lead++;
        if (lead >= len) break;
        c390243_add((k & 1) ? neg : pos, t, lead, len);
    }

    {
        int64_t borrow = 0;
        for (i = len; i > 0; i--) {
            int64_t v = (int64_t)pos[i - 1] - (int64_t)neg[i - 1] - borrow;
            if (v < 0) { v += C390243_BASE; borrow = 1; } else borrow = 0;
            pos[i - 1] = (uint32_t)v;
        }
    }
    {
        size_t p = 0, limb = 1;
        while (p < D) {
            uint32_t v = pos[limb++];
            int j;
            uint8_t tmp[9];
            for (j = 8; j >= 0; j--) { tmp[j] = (uint8_t)(v % 10); v /= 10; }
            for (j = 0; j < 9 && p < D; j++) out[p++] = tmp[j];
        }
    }
    free(t); free(pos); free(neg);
    free(c390243_digits);
    c390243_digits = out;
    c390243_have = D;
}

seq_t a(idx_t n) {
    size_t need = (size_t)n + 1;
    if (need > c390243_have) {
        size_t D = c390243_have * 2;
        if (D < need + 20) D = need + 20;
        if (D < 64) D = 64;
        c390243_compute(D);
    }
    return (seq_t)c390243_digits[n];
}
