/*
 * A374297  Number of growing self-avoiding walks of length n on a
 *          half-infinite strip of height 3 with a trapped endpoint.
 * https://oeis.org/A374297
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 21, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A374297: number of growing self‑avoiding walks of length n
   on a half‑infinite strip of height 3.
   The generating function is
        G(x) = N(x) / D(x)
   with
        N(x) = x^4*(1 + x - 2*x^2 - x^5 + x^6 - 2*x^8 - 5*x^9 - 5*x^10
                  - 2*x^11 - 2*x^12)
   and
        D(x) = (1 + x^4)*(1 - 2*x^2)*(1 - x - 2*x^3 - x^4 - 2*x^5 - 2*x^6).
   This yields a linear recurrence of order 12:
        a(n) = N_n - Σ_{k=1}^{12} d_k·a(n‑k)   (n ≥ 4)
   where d_k are the coefficients of D(x) (d_0 = 1).  The terms
   N_n are the coefficients of N(x); they are non‑zero only for
   n = 4 … 16.  The first 4 terms are a(0)=a(1)=a(2)=a(3)=0.       */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

static const int d[13] = {        /* coefficients of D(x), d[0] unused */
    0,   /* k=0 */
   -1,   /* k=1 */
   -2,   /* k=2 */
    0,   /* k=3 */
    0,   /* k=4 */
    1,   /* k=5 */
   -2,   /* k=6 */
    4,   /* k=7 */
    3,   /* k=8 */
    2,   /* k=9 */
    0,   /* k=10 */
    4,   /* k=11 */
    4    /* k=12 */
};

static const int pcoeff[17] = {   /* coefficients of N(x), 0 for n>16 */
    0, /* N0 */
    0, /* N1 */
    0, /* N2 */
    0, /* N3 */
    1, /* N4 */
    1, /* N5 */
   -2, /* N6 */
    0, /* N7 */
    0, /* N8 */
   -1, /* N9 */
    1, /* N10 */
    0, /* N11 */
   -2, /* N12 */
   -5, /* N13 */
   -5, /* N14 */
   -2, /* N15 */
   -2  /* N16 */
};

seq_t a(idx_t n)
{
    /* Memoisation cache for a[0], a[1], … up to the highest computed index. */
    static seq_t *cache = NULL;          /* array of a[i] (index 0 … ) */
    static size_t cache_size = 0;        /* capacity of the cache array */
    static idx_t last_computed = 3;      /* a[0] … a[3] are known (zero) */

    if (n < 4u) return (seq_t)0;           /* undefined below offset */

    /* Expand the cache to accommodate index n if necessary. */
    if (!cache || cache_size <= (size_t)n) {
        size_t new_size = (size_t)(n + 1);
        if (!cache) {
            cache = (seq_t *)malloc(new_size * sizeof(seq_t));
            if (!cache) return (seq_t)0;   /* allocation failure */
            for (size_t i = 0; i < new_size; ++i) cache[i] = (seq_t)0;
            cache_size = new_size;
        } else {
            seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
            if (!tmp) return (seq_t)0;   /* allocation failure */
            for (size_t i = cache_size; i < new_size; ++i) tmp[i] = (seq_t)0;
            cache = tmp;
            cache_size = new_size;
        }
    }

    /* Compute missing terms up to n. */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        if (sizeof(seq_t) <= 8) {          /* 64‑bit or smaller: use 128‑bit intermediate */
            unsigned __int128 pos_sum = 0;
            unsigned __int128 neg_sum = 0;

            for (int k = 1; k <= 12; ++k) {
                if (i < (idx_t)k) continue;
                int coeff = d[k];
                if (coeff > 0)
                    pos_sum += (unsigned __int128)cache[(size_t)(i - k)] *
                               (unsigned __int128)coeff;
                else if (coeff < 0)
                    neg_sum += (unsigned __int128)cache[(size_t)(i - k)] *
                               (unsigned __int128)(-coeff);
            }

            /* Add the N term if it exists. */
            if (i <= 16) {
                int pn = pcoeff[(size_t)i];
                if (pn >= 0)
                    neg_sum += (unsigned __int128)pn;
                else
                    pos_sum += (unsigned __int128)(-pn);
            }

            cache[(size_t)i] = (seq_t)(neg_sum - pos_sum);
        } else {                            /* arbitrary‑precision seq_t: compute directly */
            seq_t pos_sum = 0;
            seq_t neg_sum = 0;

            for (int k = 1; k <= 12; ++k) {
                if (i < (idx_t)k) continue;
                int coeff = d[k];
                seq_t val = cache[(size_t)(i - k)];
                if (coeff > 0)
                    pos_sum += val * (seq_t)coeff;
                else if (coeff < 0)
                    neg_sum += val * (seq_t)(-coeff);
            }

            if (i <= 16) {
                int pn = pcoeff[(size_t)i];
                if (pn >= 0)
                    neg_sum += (seq_t)pn;
                else
                    pos_sum += (seq_t)(-pn);
            }

            cache[(size_t)i] = neg_sum - pos_sum;
        }
    }

    last_computed = n;
    return cache[(size_t)n];
}
