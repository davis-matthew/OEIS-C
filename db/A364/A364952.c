/*
 * A364952  Dirichlet inverse of A364557, which is Möbius transform of
 *          A005941.
 * https://oeis.org/A364952
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(1) = 1, a(n) = -Sum_{d|n, d<n} A364557(n/d) * a(d), where
 *            A364557(n) = 2^(max(#2s-1,0) + Sum_{odd p^e || n}(e-1) + (pi(max
 *            odd prime)-1)), A364557(1)=1
 * Range:     i8 n <= 22, i16 n <= 58, i32 n <= 136, i64 n <= 312,
 *            i128 n <= 726, big any n
 * Validated: standard, pass, 85 terms through n = 9868 (2026-11-10)
 * Helpers:   mem, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"
#include "../utils/pow2.h"

static uint32_t *A364952_pi = NULL;
static idx_t A364952_cap = 0;

static void A364952_ensure(idx_t n)
{
    if (A364952_pi && n <= A364952_cap) return;
    idx_t cap = n < 1024 ? 1024 : n;
    unsigned char *comp = (unsigned char *)calloc((size_t)cap + 1, 1);
    uint32_t *tab = (uint32_t *)malloc(((size_t)cap + 1) * sizeof(uint32_t));
    comp[0] = 1;
    comp[1] = 1;
    for (idx_t i = 2; i * i <= cap; i++)
        if (!comp[i])
            for (idx_t j = i * i; j <= cap; j += i) comp[j] = 1;
    uint32_t c = 0;
    for (idx_t i = 0; i <= cap; i++) {
        if (!comp[i]) c++;
        tab[i] = c;
    }
    free(comp);
    free(A364952_pi);
    A364952_pi = tab;
    A364952_cap = cap;
}

static seq_t A364952_b(idx_t m)
{
    if (m == 1) return 1;
    idx_t t = m;
    idx_t ex = 0;
    while ((t & 1) == 0) { ex++; t >>= 1; }
    idx_t exponent = ex >= 1 ? ex - 1 : 0;
    idx_t maxp = 0;
    for (idx_t p = 3; p * p <= t; p += 2) {
        if (t % p == 0) {
            idx_t e = 0;
            while (t % p == 0) { e++; t /= p; }
            exponent += e - 1;
            if (p > maxp) maxp = p;
        }
    }
    if (t > 1) {
        if (t > maxp) maxp = t;
    }
    if (maxp > 0) exponent += (idx_t)A364952_pi[maxp] - 1;
    return pow2(exponent);
}

seq_t a(idx_t n)
{
    if (n == 1) return 1;
    A364952_ensure(n);

    idx_t cnt = 0;
    for (idx_t d = 1; d * d <= n; d++)
        if (n % d == 0) cnt += (d * d == n) ? 1 : 2;

    idx_t *div = (idx_t *)malloc((size_t)cnt * sizeof(idx_t));
    idx_t lo = 0, hi = cnt;
    for (idx_t d = 1; d * d <= n; d++) {
        if (n % d == 0) {
            div[lo++] = d;
            if (d * d != n) div[--hi] = n / d;
        }
    }

    seq_t *vals = mem_new(seq_t, (size_t)cnt);
    for (idx_t i = 0; i < cnt; i++) {
        idx_t d = div[i];
        if (d == 1) { vals[i] = 1; continue; }
        seq_t s = 0;
        for (idx_t j = 0; j < i; j++) {
            if (d % div[j] == 0)
                s += A364952_b(d / div[j]) * vals[j];
        }
        vals[i] = -s;
    }
    seq_t res = vals[cnt - 1];
    mem_free(vals);
    free(div);
    return res;
}
