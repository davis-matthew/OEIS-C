/*
 * A260721  Half the difference between n-th odd Ludic and n-th Lucky number:
 *          a(1) = 0; a(n) = (A003309(n+1) - A000959(n))/2.
 * https://oeis.org/A260721
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(1) = 0; a(n) = (A003309(n+1) - A000959(n))/2, computed by
 *            sieving (Ludic and Lucky sieves)
 * Range:     i8 n <= 27, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Validated: standard, pass, 112 terms through n = 9488 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static void A003309_fw_dec(uint32_t *t, size_t m, size_t p)
{
    while (p <= m) {
        t[p]--;
        p += p & (~p + 1);
    }
}

static size_t A003309_fw_find(const uint32_t *t, size_t m, size_t top, uint64_t k)
{
    size_t pos = 0;
    size_t step;
    for (step = top; step > 0; step >>= 1) {
        size_t nx = pos + step;
        if (nx <= m && t[nx] < k) {
            pos = nx;
            k -= t[nx];
        }
    }
    return pos + 1;
}

static uint64_t *A003309_vals = NULL;
static size_t A003309_cnt = 0;
static uint64_t A003309_built = 0;

static void A003309_build(uint64_t L)
{
    size_t m = (size_t)(L - 1);
    uint32_t *t = (uint32_t *)malloc((m + 1) * sizeof(uint32_t));
    uint64_t *out = (uint64_t *)malloc((m + 2) * sizeof(uint64_t));
    size_t i, top, cnt = 0;
    uint64_t rem = m;

    t[0] = 0;
    for (i = 1; i <= m; i++) t[i] = (uint32_t)(i & (~i + 1));
    top = 1;
    while (top * 2 <= m) top *= 2;

    out[cnt++] = 1;
    while (rem > 0) {
        size_t p = A003309_fw_find(t, m, top, 1);
        uint64_t k = (uint64_t)p + 1;
        uint64_t idx;
        out[cnt++] = k;
        A003309_fw_dec(t, m, p);
        rem--;
        idx = k;
        while (idx <= rem) {
            size_t q = A003309_fw_find(t, m, top, idx);
            A003309_fw_dec(t, m, q);
            rem--;
            idx += k - 1;
        }
    }
    free(t);
    free(A003309_vals);
    A003309_vals = out;
    A003309_cnt = cnt;
    A003309_built = L;
}

static uint64_t A003309_a(uint64_t n)
{
    while ((uint64_t)A003309_cnt < n) {
        uint64_t L = n * 20 + 100;
        if (L < A003309_built * 2) L = A003309_built * 2;
        A003309_build(L);
    }
    return A003309_vals[n - 1];
}

static uint64_t A000959_a(uint64_t n) {
    uint64_t max_val = n * 16;
    if (max_val < 1000) max_val = 1000;

    for (;;) {
        uint64_t cnt = (max_val + 1) / 2;
        uint64_t *lk = (uint64_t *)malloc(cnt * sizeof(uint64_t));
        uint64_t i, k;
        for (i = 0; i < cnt; i++) lk[i] = 2 * i + 1;

        for (k = 1; k < cnt; k++) {
            uint64_t step = lk[k];
            uint64_t w;
            if (step > cnt || step > n) break;
            w = step - 1;
            for (i = step - 1; i < cnt; i++) {
                if ((i + 1) % step != 0) lk[w++] = lk[i];
            }
            cnt = w;
        }

        if (n <= cnt) {
            uint64_t r = lk[n - 1];
            free(lk);
            return r;
        }
        free(lk);
        max_val *= 2;
    }
}

seq_t a(idx_t n)
{
    uint64_t L, K;
    if (n <= 1) return (seq_t)0;
    L = A003309_a((uint64_t)n + 1);
    K = A000959_a((uint64_t)n);
    return ((seq_t)L - (seq_t)K) / 2;
}
