/*
 * A347084  Dirichlet inverse of A129283, n + A003415(n).
 * https://oeis.org/A347084
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(1) = 1; a(n) = -Sum_{d|n, d<n} a(d) * A129283(n/d), where
 *            A129283(m) = m + m' (arithmetic derivative)
 * Range:     i8 n <= 65, i16 n <= 8609, i32 n <= 16384, i64 n <= 16384,
 *            i128 n <= 16384, big any n
 * Validated: standard, pass, 90 terms through n = 9535 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/mem.h"

static seq_t A129283_b(idx_t n)
{
    seq_t N = (seq_t)n;
    seq_t d = 0;
    idx_t tmp = n;
    for (idx_t p = 2; p <= tmp / p; p++) {
        if (tmp % p == 0) {
            int e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                e++;
            }
            d += (seq_t)e * (N / (seq_t)p);
        }
    }
    if (tmp > 1) {
        d += N / (seq_t)tmp;
    }
    return N + d;
}

static size_t A347084_find(const idx_t *divs, size_t cnt, idx_t v)
{
    size_t lo = 0, hi = cnt;
    while (lo < hi) {
        size_t mid = lo + (hi - lo) / 2;
        if (divs[mid] < v) lo = mid + 1;
        else hi = mid;
    }
    return lo;
}

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)1;

    size_t cnt = 0;
    for (idx_t i = 1; i <= n / i; i++) {
        if (n % i == 0) cnt += (i == n / i) ? 1 : 2;
    }

    idx_t *divs = mem_new(idx_t, cnt);
    seq_t *bv = mem_new(seq_t, cnt);
    seq_t *av = mem_new(seq_t, cnt);

    size_t lo = 0, hi = 0;
    for (idx_t i = 1; i <= n / i; i++) {
        if (n % i == 0) {
            divs[lo++] = i;
            if (i != n / i) divs[cnt - 1 - hi++] = n / i;
        }
    }

    for (size_t i = 0; i < cnt; i++) bv[i] = A129283_b(divs[i]);

    av[0] = (seq_t)1;
    for (size_t i = 1; i < cnt; i++) {
        seq_t s = 0;
        for (size_t j = 0; j < i; j++) {
            if (divs[i] % divs[j] == 0) {
                size_t k = A347084_find(divs, cnt, divs[i] / divs[j]);
                s += av[j] * bv[k];
            }
        }
        av[i] = -s;
    }

    seq_t result = av[cnt - 1];
    mem_free(divs);
    mem_free(bv);
    mem_free(av);
    return result;
}
