/*
 * A378533  Dirichlet convolution of A323910 and A378542.
 * https://oeis.org/A378533
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} A323910(d)*A378542(n/d), computed over the
 *            divisors of n only
 * Range:     u8 n <= 143, u16 n <= 8639, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 111 terms through n = 9340 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/mem.h"

static int cmp_idx_A378533(const void *x, const void *y)
{
    idx_t a = *(const idx_t *)x, b = *(const idx_t *)y;
    return (a > b) - (a < b);
}

static size_t find_A378533(const idx_t *d, size_t D, idx_t v)
{
    size_t lo = 0, hi = D;
    while (lo + 1 < hi) {
        size_t mid = (lo + hi) / 2;
        if (d[mid] <= v) lo = mid; else hi = mid;
    }
    return lo;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    idx_t pr[64]; int ex[64]; int cnt = 0;
    idx_t t = n;
    for (idx_t p = 2; p * p <= t; ++p) {
        if (t % p == 0) {
            int e = 0;
            while (t % p == 0) { t /= p; ++e; }
            pr[cnt] = p; ex[cnt] = e; ++cnt;
        }
    }
    if (t > 1) { pr[cnt] = t; ex[cnt] = 1; ++cnt; }

    size_t D = 1;
    for (int i = 0; i < cnt; ++i) D *= (size_t)(ex[i] + 1);
    idx_t *div = (idx_t *)malloc(D * sizeof(idx_t));
    int *om = (int *)malloc(D * sizeof(int));
    if (!div || !om) { free(div); free(om); return 0; }
    size_t sz = 1;
    div[0] = 1;
    for (int i = 0; i < cnt; ++i) {
        size_t cur = sz;
        idx_t pw = 1;
        for (int k = 1; k <= ex[i]; ++k) {
            pw *= pr[i];
            for (size_t j = 0; j < cur; ++j) div[sz++] = div[j] * pw;
        }
    }
    qsort(div, D, sizeof(idx_t), cmp_idx_A378533);

    seq_t *sig = mem_new(seq_t, D);
    seq_t *a323 = mem_new(seq_t, D);
    seq_t *g = mem_new(seq_t, D);
    for (size_t j = 0; j < D; ++j) {
        idx_t m = div[j];
        seq_t s = 1;
        int o = 0;
        for (int i = 0; i < cnt; ++i) {
            int e = 0;
            while (m % pr[i] == 0) { m /= pr[i]; ++e; }
            o += e;
            seq_t term = 1, tot = 1;
            for (int k = 0; k < e; ++k) { term *= (seq_t)pr[i]; tot += term; }
            s *= tot;
        }
        sig[j] = s;
        om[j] = o;
    }

    a323[0] = 1;
    for (size_t j = 1; j < D; ++j) {
        seq_t sum = 0;
        for (size_t i = 0; i < j; ++i) {
            if (div[j] % div[i]) continue;
            idx_t q = div[j] / div[i];
            size_t k = find_A378533(div, D, q);
            seq_t f = (seq_t)2 * (seq_t)q - sig[k];
            sum += f * a323[i];
        }
        a323[j] = (seq_t)0 - sum;
    }

    for (size_t k = 0; k < D; ++k) {
        seq_t s = 0;
        for (size_t i = 0; i <= k; ++i) {
            if (div[k] % div[i]) continue;
            if (((om[k] - om[i]) & 1) == 0) s += (seq_t)div[i];
        }
        g[k] = s;
    }

    seq_t result = 0;
    for (size_t j = 0; j < D; ++j) {
        size_t k = find_A378533(div, D, n / div[j]);
        result += a323[j] * g[k];
    }

    mem_free(sig); mem_free(a323); mem_free(g);
    free(div); free(om);
    return result;
}
