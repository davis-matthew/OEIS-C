/*
 * A347098  a(1) = 1; a(n) = -Sum_{d|n, d < n} A336853(n/d) * a(d), where
 *          A336853(n) = A003961(n) - n.
 * https://oeis.org/A347098
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(1) = 1; a(n) = -Sum_{d|n, d<n} B(n/d)*a(d), B(k) = A003961(k)
 *            - k
 * Range:     i8 n <= 47, i16 n <= 1007, i32 n <= 16384, i64 n <= 16384,
 *            i128 n <= 16384, big any n
 * Validated: standard, pass, 88 terms through n = 9214 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

static bool A347098_isp(uint64_t v)
{
    if (v < 2) return false;
    if (v < 4) return true;
    if ((v & 1) == 0) return false;
    for (uint64_t d = 3; d * d <= v; d += 2)
        if (v % d == 0) return false;
    return true;
}

static uint64_t A347098_nextp(uint64_t p)
{
    p++;
    while (!A347098_isp(p)) p++;
    return p;
}

/* A336853(k) = A003961(k) - k */
static seq_t A347098_B(uint64_t k)
{
    seq_t r = 1;
    uint64_t m = k;
    for (uint64_t p = 2; p * p <= m; p++) {
        if (m % p == 0) {
            uint64_t q = A347098_nextp(p);
            while (m % p == 0) {
                m /= p;
                r *= (seq_t)q;
            }
        }
    }
    if (m > 1) r *= (seq_t)A347098_nextp(m);
    return r - (seq_t)k;
}

static int A347098_cmp(const void *x, const void *y)
{
    uint64_t a = *(const uint64_t *)x, b = *(const uint64_t *)y;
    return a < b ? -1 : (a > b ? 1 : 0);
}

seq_t a(idx_t n)
{
    uint64_t N = (uint64_t)n;
    if (N <= 1) return (seq_t)1;
    size_t cap = 64, cnt = 0;
    uint64_t *dv = (uint64_t *)malloc(cap * sizeof(uint64_t));
    for (uint64_t d = 1; d * d <= N; d++) {
        if (N % d == 0) {
            if (cnt + 2 > cap) {
                cap *= 2;
                dv = (uint64_t *)realloc(dv, cap * sizeof(uint64_t));
            }
            dv[cnt++] = d;
            if (d != N / d) dv[cnt++] = N / d;
        }
    }
    qsort(dv, cnt, sizeof(uint64_t), A347098_cmp);
    seq_t *val = mem_new(seq_t, cnt);
    val[0] = 1;
    for (size_t i = 1; i < cnt; i++) {
        seq_t s = 0;
        for (size_t j = 0; j < i; j++) {
            if (dv[i] % dv[j] == 0)
                s += A347098_B(dv[i] / dv[j]) * val[j];
        }
        val[i] = -s;
    }
    seq_t res = val[cnt - 1];
    mem_free(val);
    free(dv);
    return res;
}
