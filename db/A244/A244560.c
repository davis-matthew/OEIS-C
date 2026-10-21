/*
 * A244560  Expansion of f(-x^1, -x^7)^2 in powers of x where f() is
 *          Ramanujan's two-variable theta function.
 * https://oeis.org/A244560
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 80, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A244560: expansion of f(-x, -x^7)^2 where f is Ramanujan's two‑variable theta function.
   The coefficient a(n) is the convolution square of the sequence
   b(n) = Σ_{k∈ℤ} (-1)^k · 1{n = 4k^2-3k}, i.e.
   a(n) = Σ_{i=0}^n b(i)·b(n-i). */
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

/* static cache of computed a[n] values */
static seq_t *a_cache   = NULL;     /* array 0 .. cache_limit */
static size_t cache_limit = 0;

/* Recompute a[0..new_limit] */
static void compute_up_to(idx_t new_limit)
{
    size_t limit = (size_t)new_limit;

    /* allocate array for b[n] */
    seq_t *b_coef = (seq_t *)calloc(limit + 1, sizeof(seq_t));
    if (!b_coef) return;

    /* fill b[n] */
    for (idx_t k = 0; ; ++k) {
        seq_t pos = (seq_t)4 * k * k - (seq_t)3 * k;
        seq_t neg = (seq_t)4 * k * k + (seq_t)3 * k;
        if (pos > (seq_t)new_limit && neg > (seq_t)new_limit) break;
        seq_t coeff = (k & 1) ? (seq_t)-1 : (seq_t)1;
        if (pos <= (seq_t)new_limit) b_coef[(size_t)pos] += coeff;
        if (k > 0 && neg <= (seq_t)new_limit) b_coef[(size_t)neg] += coeff;
    }

    /* gather indices with non-zero coefficients */
    size_t m = 0;
    for (size_t i = 0; i <= limit; ++i) if (b_coef[i] != 0) ++m;
    size_t *idx = NULL;
    if (m > 0) {
        idx = (size_t *)calloc(m, sizeof(size_t));
        if (!idx) { free(b_coef); return; }
        size_t pos = 0;
        for (size_t i = 0; i <= limit; ++i) if (b_coef[i] != 0) idx[pos++] = i;
    }

    /* compute convolution square a[n] = Σ b[i]*b[n-i] */
    seq_t *a_new = (seq_t *)calloc(limit + 1, sizeof(seq_t));
    if (!a_new) { free(idx); free(b_coef); return; }

    for (size_t ii = 0; ii < m; ++ii) {
        size_t i = idx[ii];
        seq_t bi = b_coef[i];
        for (size_t jj = 0; jj < m; ++jj) {
            size_t j = idx[jj];
            size_t sum = i + j;
            if (sum > limit) continue;
            a_new[sum] += bi * b_coef[j];
        }
    }

    /* replace old cache */
    free(a_cache);
    a_cache = a_new;
    cache_limit = limit;

    free(idx);
    free(b_coef);
}

seq_t a(idx_t n)
{
    if (a_cache == NULL || (size_t)n > cache_limit)
        compute_up_to(n);
    return a_cache[(size_t)n];
}
