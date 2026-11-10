/*
 * A345065  Sum of A011772 and its Dirichlet inverse.
 * https://oeis.org/A345065
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = A011772(n) + A345055(n), where A345055 is the Dirichlet
 *            inverse of A011772: g(1) = 1, g(n) = -Sum_{d|n, d>1}
 *            A011772(d)*g(n/d). Computed over the divisors of n
 * Range:     i8 n <= 45, i16 n <= 1319, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Validated: standard, pass, 97 terms through n = 9403 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/mem.h"

static idx_t A011772_f(idx_t n)
{
    unsigned __int128 mod = (unsigned __int128)n * 2u;
    for (idx_t m = 1; m < 2 * n; ++m) {
        unsigned __int128 p = (unsigned __int128)m * (m + 1);
        if (p % mod == 0) return m;
    }
    return 2 * n - 1;
}

static size_t A345065_find(const idx_t *d, size_t k, idx_t v)
{
    size_t lo = 0, hi = k;
    while (lo + 1 < hi) {
        size_t mid = (lo + hi) / 2;
        if (d[mid] <= v) lo = mid; else hi = mid;
    }
    return lo;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    idx_t *d = (idx_t *)malloc(sizeof(idx_t) * 2 * 8192);
    size_t cap = 2 * 8192, k = 0, hi_cnt = 0;
    idx_t *hi = (idx_t *)malloc(sizeof(idx_t) * 8192);
    size_t hcap = 8192;
    (void)cap;
    for (idx_t i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            d[k++] = i;
            if (i != n / i) {
                if (hi_cnt >= hcap) { hcap *= 2; hi = (idx_t *)realloc(hi, sizeof(idx_t) * hcap); }
                hi[hi_cnt++] = n / i;
            }
        }
        if (k >= 8000) break;
    }
    while (hi_cnt > 0) d[k++] = hi[--hi_cnt];
    free(hi);

    seq_t *fv = mem_new(seq_t, k);
    seq_t *g = mem_new(seq_t, k);
    for (size_t i = 0; i < k; ++i) fv[i] = (seq_t)A011772_f(d[i]);
    g[0] = 1;
    for (size_t i = 1; i < k; ++i) {
        seq_t s = 0;
        for (size_t j = 1; j <= i; ++j) {
            if (d[i] % d[j] == 0) {
                size_t q = A345065_find(d, k, d[i] / d[j]);
                s += fv[j] * g[q];
            }
        }
        g[i] = -s;
    }
    seq_t res = fv[k - 1] + g[k - 1];
    mem_free(fv);
    mem_free(g);
    free(d);
    return res;
}
