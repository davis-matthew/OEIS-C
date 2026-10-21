/*
 * A289636  Coefficients in expansion of -q*E'_4/E_4 where E_4 is the
 *          Eisenstein Series (A004009).
 * https://oeis.org/A289636
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = - Σ_{k=1}^{n-1} E_4(k) * a(n-k) - E_4(n) * n where E_4(k)
 *            = 240 * σ_3(k) and σ_3(k) = Σ_{d|k} d^3 (i.e. a(n) follows the
 *            recurrence given in the OEIS entry)
 * Range:     i8 none, i16 n <= 1, i32 n <= 3, i64 n <= 8, i128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* The types seq_t and idx_t are supplied by the build environment. */
static seq_t *a_cache   = NULL;   /* a(n)  */
static seq_t *sigma_cache = NULL; /* σ_3(n) */
static seq_t *e4_cache = NULL;    /* E_4(n) = 240 * σ_3(n) */
static idx_t  cache_n = 0;        /* largest n computed so far */

/* Extend caches to include all indices up to n. */
static void extend_cache(idx_t n)
{
    if (n <= cache_n) return;                 /* nothing to do */

    /* Allocate new, zero‑initialised arrays of size n+1 (indices 0..n). */
    size_t sz = (size_t)(n + 1);
    seq_t *new_a     = (seq_t *)malloc(sz * sizeof(seq_t));
    seq_t *new_sigma = (seq_t *)malloc(sz * sizeof(seq_t));
    seq_t *new_e4    = (seq_t *)malloc(sz * sizeof(seq_t));
    if (!new_a || !new_sigma || !new_e4) {
        /* In the unlikely event of allocation failure, abort. */
        abort();
    }
    memset(new_a,     0, sz * sizeof(seq_t));
    memset(new_sigma, 0, sz * sizeof(seq_t));
    memset(new_e4,    0, sz * sizeof(seq_t));

    /* Copy existing values into the new arrays. */
    if (cache_n > 0) {
        memcpy(new_a,     a_cache,     (size_t)(cache_n + 1) * sizeof(seq_t));
        memcpy(new_sigma, sigma_cache, (size_t)(cache_n + 1) * sizeof(seq_t));
        memcpy(new_e4,    e4_cache,    (size_t)(cache_n + 1) * sizeof(seq_t));
    }

    /* Compute σ_3 values for all indices up to n. */
    for (idx_t i = 1; i <= n; ++i) {
        seq_t i3 = (seq_t)((seq_t)i * (seq_t)i * (seq_t)i); /* i^3 */
        idx_t start;
        if (i <= cache_n) {
            /* First multiple of i that is > cache_n. */
            start = ((cache_n / i) + 1) * i;
        } else {
            start = i;
        }
        for (idx_t m = start; m <= n; m += i) {
            new_sigma[m] += i3;
        }
    }

    /* Compute E_4 values: E_4(k) = 240 * σ_3(k). */
    for (idx_t i = 1; i <= n; ++i) {
        new_e4[i] = (seq_t)240 * new_sigma[i];
    }

    /* Compute a(n) by the recurrence for all new indices. */
    for (idx_t i = cache_n + 1; i <= n; ++i) {
        seq_t sum = 0;
        for (idx_t k = 1; k < i; ++k) {
            sum += new_e4[k] * new_a[i - k];
        }
        new_a[i] = -sum - new_e4[i] * (seq_t)i;
    }

    /* Replace old arrays with the new ones. */
    free(a_cache);
    free(sigma_cache);
    free(e4_cache);
    a_cache     = new_a;
    sigma_cache = new_sigma;
    e4_cache    = new_e4;
    cache_n = n;
}

/* Public function as required by the specification. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;           /* out‑of‑range indices map to zero */
    if (a_cache == NULL || n > cache_n) {
        extend_cache(n);
    }
    return a_cache[n];
}
