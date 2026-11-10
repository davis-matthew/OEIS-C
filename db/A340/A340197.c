/*
 * A340197  a(1) = 1; for n > 1, a(n) = Sum_{d|n, d<n} (A318833(n/d)-1) *
 *          A340197(d).
 * https://oeis.org/A340197
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; for n > 1, a(n) = Sum_{d|n, d<n} (A318833(n/d)-1) *
 *            a(d), where A318833(n) = n + A023900(n)
 * Range:     u8 n <= 59, u16 n <= 1079, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 98 terms through n = 9762 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"

static seq_t A318833_a(idx_t n)
{
    idx_t temp = n;
    seq_t d = 1, phi = 1;
    int omega = 0;
    for (idx_t p = 2; p <= temp / p; p++) {
        if (temp % p != 0) continue;
        idx_t e = 0;
        while (temp % p == 0) { temp /= p; e++; }
        omega++;
        phi *= (seq_t)(p - 1);
        for (idx_t i = 1; i < e; ++i) { phi *= (seq_t)p; d *= (seq_t)p; }
    }
    if (temp > 1) {
        omega++;
        phi *= (seq_t)(temp - 1);
    }
    seq_t q = phi / d;
    if (omega % 2 == 0) return (seq_t)n + q;
    return (seq_t)n - q;
}

static int A340197_cmp(const void *x, const void *y)
{
    idx_t a = *(const idx_t *)x, b = *(const idx_t *)y;
    return a < b ? -1 : (a > b ? 1 : 0);
}

static size_t A340197_find(const idx_t *dv, size_t cnt, idx_t v)
{
    size_t lo = 0, hi = cnt;
    while (lo + 1 < hi) {
        size_t mid = (lo + hi) / 2;
        if (dv[mid] <= v) lo = mid; else hi = mid;
    }
    return lo;
}

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)1;
    size_t cap = 0;
    for (idx_t i = 1; i <= n / i; i++)
        if (n % i == 0) cap += 2;
    idx_t *dv = (idx_t *)malloc(cap * sizeof(idx_t));
    size_t cnt = 0;
    for (idx_t i = 1; i <= n / i; i++) {
        if (n % i == 0) {
            dv[cnt++] = i;
            if (i != n / i) dv[cnt++] = n / i;
        }
    }
    qsort(dv, cnt, sizeof(idx_t), A340197_cmp);

    seq_t *A = mem_new(seq_t, cnt);
    seq_t *V = mem_new(seq_t, cnt);
    for (size_t k = 0; k < cnt; k++) A[k] = A318833_a(dv[k]) - 1;

    V[0] = (seq_t)1;
    for (size_t m = 1; m < cnt; m++) {
        seq_t s = 0;
        for (size_t j = 0; j < m; j++) {
            if (dv[m] % dv[j] != 0) continue;
            size_t q = A340197_find(dv, cnt, dv[m] / dv[j]);
            s += A[q] * V[j];
        }
        V[m] = s;
    }
    seq_t res = V[cnt - 1];
    mem_free(A);
    mem_free(V);
    free(dv);
    return res;
}
