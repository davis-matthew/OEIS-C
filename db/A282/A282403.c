/*
 * A282403  Coefficients in q-expansion of E_4^4*E_6^2, where E_4 and E_6 are
 *          respectively the Eisenstein series A004009 and A013973.
 * https://oeis.org/A282403
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 1, i16 n <= 1, i32 n <= 3, i64 n <= 8, i128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Coefficients in q‑expansion of E₄⁴*E₆²,
   where  E₄(q) = 1 + 240 Σ_{m≥1} σ₃(m) qᵐ  and
           E₆(q) = 1 – 504 Σ_{m≥1} σ₅(m) qᵐ ;
   a(n) = coefficient of qⁿ in (E₄(q))⁴ (E₆(q))². */

#include <stdlib.h>
#include <stdint.h>

/* Simple convolution of two power series a and b (length n+1).
   The result is written into res, which must have space for n+1
   elements. */
static void conv(const seq_t *a, const seq_t *b, idx_t n, seq_t *res)
{
    idx_t i, k;
    for (i = 0; i <= n; ++i) {
        seq_t sum = 0;
        for (k = 0; k <= i; ++k)
            sum += a[k] * b[i - k];
        res[i] = sum;
    }
}

/* Cached global data. All arrays are valid for indices 0 .. size-1. */
static seq_t *coeff  = NULL;   /* final coefficients a(n) */
static idx_t  size   = 0;      /* current allocated size */

/* Recompute all auxiliary series up to new_n (inclusive). */
static void expand(idx_t new_n)
{
    idx_t L = new_n + 1;   /* array length */

    /* Allocate temporary arrays.  The calloc cast is required for C++.   */
    seq_t *sigma3 = (seq_t *)calloc(L, sizeof(seq_t));
    seq_t *sigma5 = (seq_t *)calloc(L, sizeof(seq_t));
    seq_t *E4     = (seq_t *)calloc(L, sizeof(seq_t));
    seq_t *E6     = (seq_t *)calloc(L, sizeof(seq_t));
    seq_t *e4_2   = (seq_t *)calloc(L, sizeof(seq_t));
    seq_t *e6_2   = (seq_t *)calloc(L, sizeof(seq_t));
    seq_t *e4_4   = (seq_t *)calloc(L, sizeof(seq_t));
    seq_t *tmp    = (seq_t *)calloc(L, sizeof(seq_t));      /* final result */

    /* 1. Compute σ₃(n) and σ₅(n) up to new_n. */
    idx_t i, j;
    for (i = 1; i <= new_n; ++i) {
        seq_t icub = (seq_t)i * (seq_t)i * (seq_t)i;     /* i³   */
        seq_t i5   = icub * (seq_t)i * (seq_t)i;        /* i⁵   */
        for (j = i; j <= new_n; j += i) {
            sigma3[j] += icub;
            sigma5[j] += i5;
        }
    }

    /* 2. Build E₄ and E₆ coefficients. */
    E4[0] = 1;
    E6[0] = 1;
    for (i = 1; i <= new_n; ++i) {
        E4[i] = 240 * sigma3[i];
        E6[i] = -504 * sigma5[i];
    }

    /* 3. Compute E₄², E₆², E₄⁴. */
    conv(E4, E4, new_n, e4_2);        /* E₄²  */
    conv(e4_2, e4_2, new_n, e4_4);    /* E₄⁴  */
    conv(E6, E6, new_n, e6_2);        /* E₆²  */

    /* 4. Final convolution (E₄⁴) * (E₆²). */
    conv(e4_4, e6_2, new_n, tmp);

    /* 5. Free old arrays and store new results. */
    if (coeff != NULL) free(coeff);
    free(sigma3);
    free(sigma5);
    free(E4);
    free(E6);
    free(e4_2);
    free(e6_2);
    free(e4_4);

    coeff = tmp;
    size  = L;
}

/* Public function to obtain a(n). */
seq_t a(idx_t n)
{
    if (n >= size)
        expand(n);            /* compute up to requested index */
    return coeff[n];
}
