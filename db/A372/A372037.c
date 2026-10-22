/*
 * A372037  G.f. A(x) satisfies A(x) = ( 1 + 4*x*(1 + x)*A(x) )^(1/2).
 * https://oeis.org/A372037
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) satisfies A(x) = sqrt(1 + 4*x*(1 + x)*A(x)) Recurrence
 *            derived from A^2 = 1 + 4x(1+x)A: a(0) = 1; for n ≥ 1 a(n) =
 *            (4*a(n-1) + 4*a(n-2) - Σ_{k=1}^{n-1} a(k)*a(n-k)) / 2
 * Range:     i8 n <= 9, i16 n <= 16, i32 n <= 29, i64 n <= 34, i128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0; /* how many entries are already known */

    /* ensure cache can hold indices up to n */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 1;
        while (new_cap <= (size_t)n + 1) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp && new_cap > 0) abort();       /* allocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* initialise first term if necessary */
    if (cache_len == 0) {
        cache[0] = (seq_t)1;
        cache_len = 1;
    }

    /* compute missing values in increasing order */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        seq_t B = (seq_t)4 * cache[i - 1];
        if (i >= 2) B += (seq_t)4 * cache[i - 2];

        seq_t S = 0;
        for (size_t k = 1; k < i; ++k) {
            S += cache[k] * cache[i - k];
        }

        cache[i] = (B - S) / 2;  /* exact division by 2 */
    }

    cache_len = (size_t)n + 1;
    return cache[(size_t)n];
}
