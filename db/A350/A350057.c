/*
 * A350057  Short leg lengths of integer-sided triangles with hypotenuse
 *          A084645(n).
 * https://oeis.org/A350057
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = shorter leg of the unique integer right triangle with
 *            hypotenuse c = A084645(n); c has exactly one prime p == 1 (mod
 *            4), with exponent 1, and c = m*p, p = s^2+t^2, legs m*(s^2-t^2),
 *            2*m*s*t. Found by a sieve (smallest prime factor) with a
 *            doubling limit
 * Range:     u8 n <= 166, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 88 terms through n = 9934 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint32_t *g_hyp = NULL;
static uint32_t *g_p = NULL;
static size_t g_cnt = 0;
static uint32_t g_limit = 0;

static void A350057_build(uint32_t L) {
    uint32_t *spf = (uint32_t *)calloc((size_t)L + 1, sizeof(uint32_t));
    if (!spf) return;
    for (uint32_t i = 2; i <= L; i++) {
        if (spf[i] == 0) {
            for (uint32_t j = i; j <= L; j += i) {
                if (spf[j] == 0) spf[j] = i;
                if (L - j < i) break;
            }
        }
    }
    free(g_hyp);
    free(g_p);
    size_t cap = 1024;
    g_hyp = (uint32_t *)malloc(cap * sizeof(uint32_t));
    g_p = (uint32_t *)malloc(cap * sizeof(uint32_t));
    g_cnt = 0;
    for (uint32_t c = 2; c <= L; c++) {
        uint32_t x = c, cnt = 0, p = 0;
        bool ok = true;
        while (x > 1) {
            uint32_t q = spf[x];
            int e = 0;
            while (x % q == 0) { x /= q; e++; }
            if (q % 4 == 1) {
                cnt++;
                if (e > 1 || cnt > 1) { ok = false; break; }
                p = q;
            }
        }
        if (ok && cnt == 1) {
            if (g_cnt >= cap) {
                cap *= 2;
                g_hyp = (uint32_t *)realloc(g_hyp, cap * sizeof(uint32_t));
                g_p = (uint32_t *)realloc(g_p, cap * sizeof(uint32_t));
            }
            g_hyp[g_cnt] = c;
            g_p[g_cnt] = p;
            g_cnt++;
        }
    }
    g_limit = L;
    free(spf);
}

seq_t a(idx_t n) {
    if (g_limit == 0) A350057_build(1u << 14);
    while ((idx_t)g_cnt < n) A350057_build(g_limit * 2);
    size_t i = (size_t)(n - 1);
    uint64_t c = g_hyp[i], p = g_p[i], m = c / p;
    uint64_t best = 0;
    for (uint64_t t = 1; 2 * t * t < p; t++) {
        uint64_t s2 = p - t * t;
        uint64_t s = 0;
        while ((s + 1) * (s + 1) <= s2) s++;
        if (s * s == s2) {
            uint64_t l1 = m * (s * s - t * t);
            uint64_t l2 = m * 2 * s * t;
            best = l1 < l2 ? l1 : l2;
            break;
        }
    }
    return (seq_t)best;
}
