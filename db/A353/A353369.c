/*
 * A353369  Sum of A103391 ("even fractal sequence") and its Dirichlet
 *          inverse.
 * https://oeis.org/A353369
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = A103391(n) + A353368(n), where A353368 is the Dirichlet
 *            inverse of A103391: b(1) = 1, b(n) = -Sum_{d|n, d>1}
 *            A103391(d)*b(n/d); computed over the divisors of n
 * Range:     i8 n <= 125, i16 n <= 9701, i32 n <= 16384, i64 n <= 16384,
 *            i128 n <= 16384, big any n
 * Validated: standard, pass, 106 terms through n = 9441 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/mem.h"

static seq_t A103391_a(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;
    idx_t t = n - 1;
    while ((t & (idx_t)1) == 0)
        t >>= 1;
    idx_t v = (t + (idx_t)3) / (idx_t)2;
    return (seq_t)v;
}

static size_t A353369_find(const idx_t *divs, size_t cnt, idx_t x)
{
    size_t lo = 0, hi = cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) / 2;
        if (divs[mid] < x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;
    size_t cnt = 0;
    idx_t i;
    for (i = 1; i <= n / i; i++)
        if (n % i == 0)
            cnt += (i == n / i) ? 1 : 2;

    idx_t *divs = (idx_t *)malloc(cnt * sizeof(idx_t));
    size_t lo = 0, hi = cnt;
    for (i = 1; i <= n / i; i++) {
        if (n % i == 0) {
            divs[lo++] = i;
            if (i != n / i)
                divs[--hi] = n / i;
        }
    }
    /* divs[0..cnt) now ascending: small ones ascending, large ones descending placed from the end */
    seq_t *b = mem_new(seq_t, cnt);
    size_t k, j;
    for (k = 0; k < cnt; k++) {
        idx_t m = divs[k];
        if (m == 1) {
            b[k] = (seq_t)1;
            continue;
        }
        seq_t s = (seq_t)0;
        for (j = 1; j <= k; j++) {
            idx_t d = divs[j];
            if (m % d == 0) {
                size_t q = A353369_find(divs, cnt, m / d);
                s += A103391_a(d) * b[q];
            }
        }
        b[k] = -s;
    }
    seq_t res = A103391_a(n) + b[cnt - 1];
    mem_free(b);
    free(divs);
    return res;
}
