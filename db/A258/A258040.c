/*
 * A258040  Expansion of f(x) / f(-x) in powers of x where f() is the g.f. for
 *          A007325.
 * https://oeis.org/A258040
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   * Let b(i) be the period‑20 sequence * b = {-2, 1, 2, 0, 0, -1,
 *            2, 0, -2, 0, * -2, 0, 2, -1, 0, 0, 2, 1, -2, 0}. * For k ≥ 1
 *            define * g(k) = Σ_{d|k} d * b(d‑1). * The required sequence
 *            satisfies * a(0) = 1 ; * for n ≥ 1 a(n) = (1/n) * Σ_{k=1}^{n}
 *            g(k) * a(n‑k). * This is the Euler transform of th...
 * Range:     i8 n <= 18, i16 n <= 60, i32 n <= 314, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/* Period‑20 sequence b */
static const int periodic[20] = {
    -2, 1, 2, 0, 0, -1, 2, 0, -2, 0,
    -2, 0, 2, -1, 0, 0, 2, 1, -2, 0
};

/* Static cache of a(0)..a(max) */
static seq_t *cache = NULL;
static size_t cache_cap = 0;    /* allocated capacity */
static idx_t  cache_len = 0;    /* number of computed elements */

/* Ensure that a(0)..a(n) are computed */
static void ensure_a(idx_t n)
{
    if (n < cache_len) return;

    size_t new_len = (size_t)(n + 1);
    if (new_len > cache_cap) {
        size_t new_cap = new_len * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    /* initialise a(0) if not done yet */
    if (cache_len == 0) {
        cache[0] = 1;
        cache_len = 1;
    }

    /* compute g[1..n] */
    int64_t *g = (int64_t *)calloc(new_len, sizeof(int64_t));
    if (!g) abort();

    for (int64_t d = 1; d <= (int64_t)n; ++d) {
        int64_t val = d * (int64_t)periodic[(d - 1) % 20];
        for (idx_t m = (idx_t)d; m <= n; m += d) {
            g[m] += val;
        }
    }

    /* recurrence a(n) = (1/n) Σ_{k=1}^n g(k) * a(n-k) */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t sum = 0;
        for (idx_t k = 1; k <= i; ++k) {
            sum += cache[i - k] * (seq_t)g[k];
        }
        cache[i] = sum / (seq_t)i;
    }

    free(g);
    cache_len = n + 1;
}

seq_t a(idx_t n)
{
    ensure_a(n);
    return cache[n];
}
