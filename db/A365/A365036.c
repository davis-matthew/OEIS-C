/*
 * A365036  E.g.f. satisfies A(x) = exp(x * (1 + x/A(x)^2)).
 * https://oeis.org/A365036
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 4, i16 n <= 6, i32 n <= 10, i64 n <= 16, i128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula (OEIS notation):
 *     a(n) = n! * Sum_{k=0..n} (-2*n+2*k+1)^(k-1) * binomial(k, n-k)/k!
 *
 * Implementation strategy:
 *   A(x) satisfies A = exp( x + x^2 / A^2 ).  Using the standard
 *   recurrence for the exponential of a power series we obtain
 *   a[n] = Σ_{k=1..n} binom(n-1,k-1) * f[k] * a[n-k]
 *   where f[1] = 1 and for k ≥ 2
 *          f[k] = k*(k-1) * q[k-2]
 *   with q being the coefficients of 1 / A^2.
 *
 *   The coefficients of 1 / A^2 are computed from the coefficients
 *   g2 of A^2 using the binomial-inversion identity
 *       q[n] = - Σ_{k=1..n} binom(n,k) * g2[k] * q[n-k].
 *
 *   The coefficients of A^2 are obtained directly from the
 *   convolution of A with itself:
 *       g2[n] = Σ_{i=0..n} binom(n,i) * a[i] * a[n-i].
 *
 *   The routine keeps static cached arrays so that multiple calls
 *   with arbitrary n (not necessarily increasing) are answered
 *   efficiently.  All arithmetic is performed with the type seq_t,
 *   which may be a fixed-width signed integer or an arbitrary-precision
 *   integer class; the same code compiles under C11 and C++17.
 */

#include <stdlib.h>        /* malloc, realloc, free */
#include <stddef.h>        /* size_t */

seq_t a(idx_t n)
{
    /* ---------- static cache of computed values ---------- */
    static seq_t *a_cache = NULL;
    static seq_t *g2_cache = NULL;
    static seq_t *q_cache = NULL;
    static idx_t cache_max = (idx_t)-1;          /* no values cached yet */
    static size_t cache_cap = 0;                 /* allocated capacity */

    /* ---------- initialise cache for n == 0 ---------- */
    if (cache_cap == 0) {
        cache_cap = 1;                            /* at least space for m=0 */
        a_cache   = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        g2_cache  = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        q_cache   = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!a_cache || !g2_cache || !q_cache) {
            /* allocation failure – return 0 (unspecified) */
            free(a_cache); free(g2_cache); free(q_cache);
            return (seq_t)0;
        }
        a_cache[0]  = (seq_t)1;                   /* a[0] = 1 */
        g2_cache[0] = (seq_t)1;                   /* A^2 coefficient 0 = 1^2 */
        q_cache[0]  = (seq_t)1;                   /* reciprocal of 1 is 1 */
        cache_max = 0;
    }

    /* ---------- extend cache up to requested n ---------- */
    if (n > cache_max) {
        /* grow capacity as needed */
        size_t new_cap = cache_cap;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp;
        tmp = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
        if (!tmp) goto error;
        a_cache = tmp;
        tmp = (seq_t *)realloc(g2_cache, new_cap * sizeof(seq_t));
        if (!tmp) goto error;
        g2_cache = tmp;
        tmp = (seq_t *)realloc(q_cache, new_cap * sizeof(seq_t));
        if (!tmp) goto error;
        q_cache = tmp;
        cache_cap = new_cap;

        /* fill new slots (uninitialized so set to zero for safety) */
        for (size_t i = cache_max + 1; i <= (size_t)n; ++i) {
            a_cache[i]  = (seq_t)0;
            g2_cache[i] = (seq_t)0;
            q_cache[i]  = (seq_t)0;
        }

        /* ---------- iterative computation for each new index ---------- */
        for (size_t m = cache_max + 1; m <= (size_t)n; ++m) {

            /* ---- compute a[m] ---- */
            seq_t sum_a = (seq_t)0;
            seq_t bin = (seq_t)1;                 /* binom(m-1,0) */
            const idx_t r = (idx_t)m - 1;

            for (idx_t k = 1; k <= (idx_t)m; ++k) {
                /* determine f[k] */
                seq_t f_k;
                if (k == 1) {
                    f_k = (seq_t)1;
                } else {
                    /* k*(k-1) * q[k-2] */
                    seq_t tmp = (seq_t)k * (seq_t)(k - 1);
                    tmp *= q_cache[(size_t)(k - 2)];
                    f_k  = tmp;
                }

                seq_t term = bin * f_k * a_cache[(size_t)(m - k)];
                sum_a += term;

                /* update binom(m-1,k) for next iteration */
                if (k < (idx_t)m) {
                    /* bin = bin * (r - k + 1) / k */
                    bin = bin * (seq_t)(r - k + 1) / (seq_t)k;
                }
            }
            a_cache[(size_t)m] = sum_a;

            /* ---- compute g2[m] ---- */
            seq_t sum_g2 = (seq_t)0;
            seq_t bin_g2 = (seq_t)1;                     /* binom(m,0) */
            const idx_t r2 = (idx_t)m;                   /* r = m for binomial(m,k) */

            for (idx_t i = 0; i <= (idx_t)m; ++i) {
                seq_t term_g2 = bin_g2 * a_cache[(size_t)i] * a_cache[(size_t)(m - i)];
                sum_g2 += term_g2;

                /* update binom(m,i+1) for next i */
                if (i < (idx_t)m) {
                    /* bin = bin * (m - i) / (i + 1) */
                    bin_g2 = bin_g2 * (seq_t)(r2 - i) / (seq_t)(i + 1);
                }
            }
            g2_cache[(size_t)m] = sum_g2;

            /* ---- compute q[m] ---- */
            seq_t sum_q = (seq_t)0;
            seq_t bin_q = (seq_t)1;                 /* binom(m,0) */

            for (idx_t k = 1; k <= (idx_t)m; ++k) {
                /* move to binom(m,k) */
                bin_q = bin_q * (seq_t)(r2 - k + 1) / (seq_t)k;

                seq_t term_q = bin_q * g2_cache[(size_t)k] * q_cache[(size_t)(m - k)];
                sum_q += term_q;
            }

            q_cache[(size_t)m] = -sum_q;
        }
        cache_max = n;
    }

    return a_cache[(size_t)n];

error:
    free(a_cache);
    free(g2_cache);
    free(q_cache);
    return (seq_t)0;
}
