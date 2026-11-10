/*
 * A260948  Coefficients in asymptotic expansion of sequence A259870.
 * https://oeis.org/A260948
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{j=0..n} Stirling2(n,j)*g(j), where g(0)=1 and k*g(k)
 *            = 2*Sum_{m=2..k+1} A259870(m)*g(k+1-m) - (k-1)*g(k-1) for k >= 1
 *            (the A259870 series linearised about its divergent asymptotic
 *            series)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 19, u128 n <= 21,
 *            big any n
 * Validated: partial, pass, 22 terms through n = 21 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

static seq_t A259870_a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;

    if (cache_cap <= (size_t)n) {
        size_t new_cap = cache_cap == 0 ? 1 : cache_cap * 2;
        if (new_cap <= (size_t)n + 1)
            new_cap = (size_t)n + 1;
        seq_t *new_cache = mem_resize(seq_t, cache, new_cap);
        if (!new_cache) {
            exit(EXIT_FAILURE);
        }
        cache = new_cache;
        for (size_t i = cache_cap; i < new_cap; ++i) {
            if (i == 0) {
                cache[i] = 0;
            } else if (i == 1) {
                cache[i] = 1;
            } else {
                seq_t sum = 0;
                for (idx_t j = 1; j < i; ++j) {
                    sum += cache[j] * cache[i - j];
                }
                cache[i] = (seq_t)i * cache[i - 1] +
                           (seq_t)(i - 2) * cache[i - 2] -
                           sum;
            }
        }
        cache_cap = new_cap;
    }
    return cache[n];
}

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    size_t N = (size_t)n;

    seq_t *g = mem_new(seq_t, N + 1);
    seq_t *s = mem_new(seq_t, N + 1);
    if (!g || !s) exit(EXIT_FAILURE);

    g[0] = 1;
    for (size_t k = 1; k <= N; ++k) {
        seq_t sum = 0;
        for (size_t m = 2; m <= k + 1; ++m) {
            sum += A259870_a((idx_t)m) * g[k + 1 - m];
        }
        seq_t t = (seq_t)2 * sum - (seq_t)(k - 1) * g[k - 1];
        g[k] = t / (seq_t)k;
    }

    /* Stirling numbers of the second kind, row N, built in place */
    s[0] = 1;
    for (size_t r = 1; r <= N; ++r) {
        for (size_t j = r; j >= 1; --j) {
            s[j] = (seq_t)j * s[j] + s[j - 1];
        }
        s[0] = 0;
    }

    seq_t res = 0;
    for (size_t j = 0; j <= N; ++j) {
        res += s[j] * g[j];
    }

    mem_free(g);
    mem_free(s);
    return res;
}
