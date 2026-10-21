/*
 * A263918  Riordan array (f(x)^4, f(x)), where 1 + x*f^4(x)/(1 - x*f(x)) =
 *          f(x).
 * https://oeis.org/A263918
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy, changed
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 20, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A263918 – flattened triangle (f^4 , f)
 *  a(n) = coeff of x^(r-k) in f^(4+k) where
 *  r = max t such that t*(t+1)/2 <= n
 *  k = n - r*(r+1)/2
 *
 *  The power series f satisfies
 *     f(0) = 1
 *     for n>=1: f[n] = (1/n) * sum_{k=1..n} binomial(n,k) * binomial(n+3*k, k-1)
 *
 *  The sequence is computed from the Riordan array (f^4, f).
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/binomial.h"

/* ----------------------------------------------------------- */
/*  Global data                                              */
/* ----------------------------------------------------------- */
static seq_t *f_coeff   = NULL;   /* coefficients of f   */
static idx_t  f_size    = 0;      /* number of coefficients stored (f_size == size of f_coeff) */

static seq_t **pow_arr  = NULL;   /* pow_arr[p] = coefficient array of f^p,  p >= 4      */
static idx_t  pow_max_pow = 0;   /* largest exponent stored  (size of pow_arr-1)        */
static idx_t  pow_max_deg = 0;   /* largest degree stored for each power (max of all)  */

/* ----------------------------------------------------------- */
/*  Polynomial multiplication  (degree <= max_deg)            */
/* ----------------------------------------------------------- */
static void poly_mul(const seq_t *a, const seq_t *b, size_t max_deg, seq_t *res)
{
    /* res must be zero‑initialized before calling   */
    for (size_t i = 0; i <= max_deg; ++i) {
        if (a[i] == 0) continue;
        for (size_t j = 0; j + i <= max_deg; ++j) {
            if (b[j] == 0) continue;
            res[i + j] += a[i] * b[j];
        }
    }
}

/* ----------------------------------------------------------- */
/*  Compute coefficients of f up to degree e                  */
/* ----------------------------------------------------------- */
static void ensure_f_coeff(idx_t e)
{
    if (e < f_size) return;               /* already computed */

    size_t new_size = e + 1;
    seq_t *tmp = (seq_t *)realloc(f_coeff, new_size * sizeof(seq_t));
    if (!tmp && new_size > 0) abort();    /* allocation failed */
    f_coeff = tmp;

    for (idx_t n = f_size; n <= e; ++n) {
        if (n == 0) {
            f_coeff[0] = (seq_t)1;
            continue;
        }
        seq_t sum = 0;
        for (idx_t k = 1; k <= n; ++k) {
            seq_t b1 = binomial(n, k);
            seq_t b2 = binomial(n + 3 * k, k - 1);
            sum += b1 * b2;
        }
        f_coeff[n] = sum / (seq_t)n;      /* division exact by construction */
    }
    f_size = new_size;
}

/* ----------------------------------------------------------- */
/*  Build power tables for all exponents up to r+4             */
/* ----------------------------------------------------------- */
static void recompute_powers(idx_t r)
{
    idx_t pmax = r + 4;                    /* maximal exponent needed   */

    /* free previous tables   */
    if (pow_arr) {
        for (idx_t p = 4; p <= pow_max_pow; ++p)
            free(pow_arr[p]);
        free(pow_arr);
    }

    pow_arr = (seq_t **)calloc(pmax + 1, sizeof(seq_t *));
    pow_max_pow = pmax;
    pow_max_deg = r;

    /* ensure f_coeff up to degree r   */
    ensure_f_coeff(r);

    /* temporary arrays for intermediate powers */
    seq_t *f2 = (seq_t *)calloc(r + 1, sizeof(seq_t));
    poly_mul(f_coeff, f_coeff, r, f2);

    seq_t *f3 = (seq_t *)calloc(r + 1, sizeof(seq_t));
    poly_mul(f2, f_coeff, r, f3);

    seq_t *f4 = (seq_t *)calloc(r + 1, sizeof(seq_t));
    poly_mul(f3, f_coeff, r, f4);
    pow_arr[4] = f4;                      /* store f^4 */

    free(f2);
    free(f3);

    /* successive powers f^5, f^6, ... , f^(pmax) */
    for (idx_t p = 5; p <= pmax; ++p) {
        seq_t *prev  = pow_arr[p - 1];
        seq_t *next  = (seq_t *)calloc(r + 1, sizeof(seq_t));
        poly_mul(prev, f_coeff, r, next);
        pow_arr[p] = next;
    }
}

/* ----------------------------------------------------------- */
/*  Find the row index r for a given flatten index n           */
/* ----------------------------------------------------------- */
static idx_t row_of(idx_t n)
{
    idx_t r = 0;
    /* find largest r with r*(r+1)/2 <= n   */
    while ((r + 1) * (r + 2) / 2 <= n) ++r;
    return r;
}

/* ----------------------------------------------------------- */
/*  Sequence entry                                           */
/* ----------------------------------------------------------- */
seq_t a(idx_t n)
{
    idx_t r = row_of(n);
    idx_t k = n - r * (r + 1) / 2;        /* column index   */
    idx_t p = 4 + k;                      /* exponent of f  */
    idx_t d = r - k;                      /* position of coefficient in that power */

    /* recompute tables if necessary   */
    if (r > pow_max_deg || p > pow_max_pow)
        recompute_powers(r);

    return pow_arr[p][d];
}
