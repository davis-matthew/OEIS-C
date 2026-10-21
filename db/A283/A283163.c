/*
 * A283163  Expansion of exp( Sum_{n>=1} -sigma(4*n)*x^n/n ) in powers of x.
 * https://oeis.org/A283163
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 5, i16 n <= 184, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A283163:  a(0)=1,  a(n) = -(1/n) * Σ_{k=1..n} sigma(4k) * a(n-k)   (n≥1) */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/sigma_of.h"   /* provides seq_t sigma_of(idx_t n) */

static seq_t *a_cache   = NULL;   /* cache of computed a(n) values   */
static size_t a_cap     = 0;       /* allocated capacity in a_cache   */
static idx_t  a_len     = 0;       /* number of entries computed      */

static seq_t *sigma_cache = NULL; /* cache of sigma(4*k) values      */
static size_t sigma_cap   = 0;     /* allocated capacity in sigma_cache */
static idx_t  sigma_len   = 0;     /* number of sigma values computed */

/* Ensure caches are large enough for indices up to n (inclusive) */
static void ensure_capacity(idx_t n)
{
    /* Resize a_cache if necessary */
    if (a_cap < (size_t)(n + 1)) {
        size_t newcap = a_cap ? a_cap * 2 : 128;
        while (newcap < (size_t)(n + 1)) newcap <<= 1;
        a_cache = (seq_t *)realloc(a_cache, newcap * sizeof(seq_t));
        a_cap   = newcap;
    }

    /* Resize sigma_cache if necessary */
    if (sigma_cap < (size_t)(n + 1)) {
        size_t newcap = sigma_cap ? sigma_cap * 2 : 128;
        while (newcap < (size_t)(n + 1)) newcap <<= 1;
        sigma_cache = (seq_t *)realloc(sigma_cache, newcap * sizeof(seq_t));
        sigma_cap   = newcap;
    }

    /* Compute missing sigma(4*k) values */
    for (idx_t k = sigma_len; k <= n; ++k) {
        sigma_cache[k] = sigma_of(4 * k);
    }
    sigma_len = n + 1;
}

/* Compute a(i) for all i in [a_len, n] and store them in a_cache */
static void compute_up_to(idx_t n)
{
    if (a_len <= n) {
        for (idx_t i = a_len; i <= n; ++i) {
            if (i == 0) {
                a_cache[0] = 1;
            } else {
                seq_t sum = 0;
                for (idx_t k = 1; k <= i; ++k) {
                    sum += sigma_cache[k] * a_cache[i - k];
                }
                a_cache[i] = -(sum / (seq_t)i);
            }
        }
        a_len = n + 1;
    }
}

/* Public entry point */
seq_t a(idx_t n)
{
    ensure_capacity(n);
    compute_up_to(n);
    return a_cache[n];
}
