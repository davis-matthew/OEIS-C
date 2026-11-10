/*
 * A323900  Sum of A287896 and its Dirichlet inverse.
 * https://oeis.org/A323900
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = A287896(n) + A323899(n), where A323899 is the Dirichlet
 *            inverse of A287896: g(1)=1, g(n) = -Sum_{d|n, d>1}
 *            A287896(d)*g(n/d). A287896(n) = (fusc(n-1)+fusc(n)+fusc(n+1))/2
 *            for n>1, A287896(1)=1
 * Range:     i8 n <= 329, i16 n <= 16384, i32 n <= 16384, i64 n <= 16384,
 *            i128 n <= 16384, big any n
 * Validated: standard, pass, 106 terms through n = 9951 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

static seq_t A287896_fusc(idx_t n)
{
    seq_t x = 1, y = 0;
    while (n > 0) {
        if (n & 1) y += x; else x += y;
        n >>= 1;
    }
    return y;
}

static seq_t A287896_a(idx_t n)
{
    if (n <= 1) return 1;
    return (A287896_fusc(n - 1) + A287896_fusc(n) + A287896_fusc(n + 1)) / 2;
}

static size_t A323900_find(const idx_t *d, size_t cnt, idx_t v)
{
    size_t lo = 0, hi = cnt - 1;
    while (lo < hi) {
        size_t mid = lo + (hi - lo) / 2;
        if (d[mid] < v) lo = mid + 1; else hi = mid;
    }
    return lo;
}

seq_t a(idx_t n)
{
    size_t cnt = 0;
    for (idx_t i = 1; i * i <= n; i++)
        if (n % i == 0) cnt += (i * i == n) ? 1 : 2;

    idx_t *divs = (idx_t *)malloc(cnt * sizeof(idx_t));
    size_t lo = 0, hi = cnt - 1;
    for (idx_t i = 1; i * i <= n; i++) {
        if (n % i == 0) {
            divs[lo++] = i;
            if (i * i != n) divs[hi--] = n / i;
        }
    }

    seq_t *F = mem_new(seq_t, cnt);
    seq_t *g = mem_new(seq_t, cnt);
    for (size_t i = 0; i < cnt; i++) F[i] = A287896_a(divs[i]);

    g[0] = 1;
    for (size_t i = 1; i < cnt; i++) {
        seq_t s = 0;
        for (size_t j = 1; j <= i; j++) {
            if (divs[i] % divs[j] == 0) {
                size_t k = A323900_find(divs, cnt, divs[i] / divs[j]);
                s += F[j] * g[k];
            }
        }
        g[i] = -s;
    }

    seq_t result = F[cnt - 1] + g[cnt - 1];
    mem_free(F);
    mem_free(g);
    free(divs);
    return result;
}
