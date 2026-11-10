/*
 * A378213  Dirichlet inverse of A140773.
 * https://oeis.org/A378213
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(1) = 1, a(n) = -Sum_{d|n, d<n} A140773(n/d) * a(d), with
 *            A140773(n) = Sum_{m|n} ceil(tau(m)/2)
 * Range:     i8 n <= 20000, i16 n <= 20000, i32 n <= 20000, i64 n <= 20000,
 *            i128 n <= 20000, big any n
 * Validated: standard, pass, 112 terms through n = 9719 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/mem.h"

static void A140773_dfs(const uint8_t *exps, size_t idx, size_t cnt,
                        seq_t d_val, seq_t *total)
{
    if (idx == cnt) {
        *total += (d_val + 1) / 2;
        return;
    }
    uint8_t e = exps[idx];
    for (uint8_t f = 0; f <= e; ++f) {
        seq_t next = d_val * (seq_t)(f + 1);
        A140773_dfs(exps, idx + 1, cnt, next, total);
    }
}

static seq_t A140773_a(uint64_t n)
{
    uint64_t temp = n;
    uint8_t exps[64];
    size_t cnt = 0;
    uint8_t e = 0;
    while (temp && (temp & 1ULL) == 0) { temp >>= 1; ++e; }
    if (e) exps[cnt++] = e;
    for (uint64_t p = 3; p <= temp / p; p += 2) {
        if (temp % p == 0) {
            e = 0;
            while (temp % p == 0) { temp /= p; ++e; }
            exps[cnt++] = e;
        }
    }
    if (temp > 1) exps[cnt++] = 1;
    seq_t total = 0;
    A140773_dfs(exps, 0, cnt, (seq_t)1, &total);
    return total;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    uint64_t N = (uint64_t)n;
    size_t cap = 64, cnt = 0;
    uint64_t *divs = (uint64_t *)malloc(cap * sizeof(uint64_t));
    for (uint64_t i = 1; i <= N / i; ++i) {
        if (N % i == 0) {
            if (cnt + 2 > cap) {
                cap *= 2;
                divs = (uint64_t *)realloc(divs, cap * sizeof(uint64_t));
            }
            divs[cnt++] = i;
            if (i != N / i) divs[cnt++] = N / i;
        }
    }
    for (size_t i = 1; i < cnt; ++i) {
        uint64_t v = divs[i];
        size_t j = i;
        while (j > 0 && divs[j - 1] > v) { divs[j] = divs[j - 1]; --j; }
        divs[j] = v;
    }
    seq_t *vals = mem_new(seq_t, cnt);
    for (size_t i = 0; i < cnt; ++i) {
        if (i == 0) { vals[0] = 1; continue; }
        seq_t s = 0;
        for (size_t j = 0; j < i; ++j) {
            if (divs[i] % divs[j] == 0)
                s += A140773_a(divs[i] / divs[j]) * vals[j];
        }
        vals[i] = -s;
    }
    seq_t res = vals[cnt - 1];
    mem_free(vals);
    free(divs);
    return res;
}
