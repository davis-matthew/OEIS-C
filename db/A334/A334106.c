/*
 * A334106  Numbers n for which A329697(n) == 6.
 * https://oeis.org/A334106
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th number k such that A329697(k) = 6, where A329697(k)
 *            = 0 if k is a power of 2, else 1 + A329697(k - k/LPF(k));
 *            computed by a sieve of largest prime factors with doubling limit
 * Range:     u8 none, u16 n <= 8485, u32 n <= 12096, u64 n <= 12096,
 *            u128 n <= 12096, big any n
 * Validated: standard, pass, 81 terms through n = 8940 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint32_t *A334106_terms = 0;
static size_t A334106_cnt = 0;
static size_t A334106_limit = 0;

static void A334106_build(size_t limit)
{
    uint32_t *lpf = (uint32_t *)calloc(limit + 1, sizeof(uint32_t));
    uint8_t *f = (uint8_t *)calloc(limit + 1, 1);
    size_t cap = 1024, cnt = 0;
    uint32_t *out = (uint32_t *)malloc(cap * sizeof(uint32_t));
    size_t p, m, k;
    for (p = 2; p <= limit; p++) {
        if (lpf[p] == 0) {
            for (m = p; m <= limit; m += p)
                lpf[m] = (uint32_t)p;
        }
    }
    for (k = 2; k <= limit; k++) {
        if ((k & (k - 1)) == 0) {
            f[k] = 0;
        } else {
            size_t nx = k - k / lpf[k];
            unsigned v = (unsigned)f[nx] + 1;
            f[k] = (uint8_t)(v > 255 ? 255 : v);
        }
        if (f[k] == 6) {
            if (cnt == cap) {
                cap *= 2;
                out = (uint32_t *)realloc(out, cap * sizeof(uint32_t));
            }
            out[cnt++] = (uint32_t)k;
        }
    }
    free(lpf);
    free(f);
    free(A334106_terms);
    A334106_terms = out;
    A334106_cnt = cnt;
    A334106_limit = limit;
}

seq_t a(idx_t n)
{
    size_t lim = A334106_limit ? A334106_limit : 4096;
    while (A334106_limit == 0 || A334106_cnt < (size_t)n) {
        if (A334106_limit != 0)
            lim = A334106_limit * 2;
        A334106_build(lim);
    }
    return (seq_t)A334106_terms[n - 1];
}
