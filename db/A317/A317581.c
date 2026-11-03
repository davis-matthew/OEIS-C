/*
 * A317581  a(1) = 1; a(n > 1) = 1 + Sum_{d|n, d<n} mu(n/d) a(d).
 * https://oeis.org/A317581
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, eigen
 * Formula:   a(n) = 1 + Sum_{d|n, d<n} mu(n/d) * a(d)
 * Range:     i8 n <= 719, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Validated: standard, pass, 96 terms through n = 7665 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/mem.h"

/* Möbius function for the divisor in the loop */
static inline int mu_of(idx_t d) {
    int n = (int)d;
    int sign = 1;
    int i = 2;
    while ((int64_t)i * i <= n) {
        if (n % i == 0) {
            int cnt = 0;
            while (n % i == 0) {
                n /= i;
                ++cnt;
                if (cnt > 1) return 0;        /* square factor */
            }
            sign = -sign;
        }
        ++i;
    }
    if (n > 1) sign = -sign;
    return sign;
}

static seq_t *a_cache = NULL;
static bool *a_computed = NULL;
static size_t cache_cap = 0;

static void ensure_cache_size(idx_t n)
{
    if (n < cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap : 64;
    while (new_cap <= (size_t)n)
        new_cap <<= 1;
    seq_t *new_cache = mem_resize(seq_t, a_cache, new_cap);
    bool *new_computed = (bool *)realloc(a_computed, new_cap * sizeof(bool));
    if (!new_cache || !new_computed) abort();
    a_cache = new_cache;
    a_computed = new_computed;
    for (size_t i = cache_cap; i < new_cap; ++i)
        a_computed[i] = false;
    cache_cap = new_cap;
}

static seq_t compute_a(idx_t n)
{
    if (n < cache_cap && a_computed[n])
        return a_cache[n];

    ensure_cache_size(n);

    if (n == 1) {
        a_cache[1] = (seq_t)1;
        a_computed[1] = true;
        return a_cache[1];
    }

    seq_t sum = 0;
    for (idx_t d = 1; d <= n / d; ++d) {
        if (n % d == 0) {
            idx_t q = n / d;
            if (d < n) {
                seq_t val_d = compute_a(d);
                sum += (seq_t)mu_of(q) * val_d;
            }
            if (q != d && q < n) {
                seq_t val_q = compute_a(q);
                sum += (seq_t)mu_of(d) * val_q;
            }
        }
    }
    seq_t res = (seq_t)1 + sum;
    a_cache[n] = res;
    a_computed[n] = true;
    return res;
}

seq_t a(idx_t n)
{
    return compute_a(n);
}
