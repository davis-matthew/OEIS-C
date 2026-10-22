/*
 * A302580  Numbers k such that the coefficient of x^k in the expansion of
 *          Ramanujan's function R(x) = Sum_{i>=0}
 *          x^(i*(i+1)/2)/Product_{j=1..i} (1 + x^j) is zero.
 * https://oeis.org/A302580
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the indices k for which the coefficient of x^k in
   R(x) = Σ_{i≥0} x^{i(i+1)/2}/∏_{j=1..i} (1+x^j) is zero.
   The function grows an internal bound until the requested element
   is known, storing all zero coefficients up to that bound. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Internal state ---------------------------------------------------------*/
static idx_t               max_k_global   = 50000;   /* current exponent bound   */
static intmax_t           *coeff          = NULL;   /* R(x) coefficients        */
static intmax_t           *H_old          = NULL;   /* H_{i-1}                  */
static intmax_t           *H_new          = NULL;   /* H_i                      */
static seq_t              *seq_zero       = NULL;   /* stored zero indices      */
static size_t              seq_zero_len   = 0;      /* number of zeros found    */

/* ------------------------------------------------------------------------*/
/* Rebuild all coefficients and zero indices up to the current bound.     */
/* All intermediate values fit in signed 64‑bit integers.                  */
static void build_sequence(void)
{
    size_t k = (size_t)max_k_global;

    /* free previous data if any ----------------------------------------*/
    free(coeff);
    free(H_old);
    free(H_new);
    free(seq_zero);

    coeff   = (intmax_t *)malloc((k + 1) * sizeof(intmax_t));
    H_old   = (intmax_t *)malloc((k + 1) * sizeof(intmax_t));
    H_new   = (intmax_t *)malloc((k + 1) * sizeof(intmax_t));
    seq_zero = (seq_t  *)malloc((k + 1) * sizeof(seq_t));

    if (!coeff || !H_old || !H_new || !seq_zero) {
        exit(EXIT_FAILURE); /* allocation failed */
    }

    /* initialise arrays -----------------------------------------------*/
    for (size_t i = 0; i <= k; ++i) {
        coeff[i] = 0;
        H_old[i] = 0;
        H_new[i] = 0;
    }
    H_old[0] = 1;   /* H_0 = 1 */

    /* main loop over i ---------------------------------------------------*/
    for (idx_t i = 0; ; ++i) {
        seq_t Ti = (seq_t)i * ((seq_t)i + 1) / 2;  /* triangular number  */
        if (Ti > max_k_global) break;              /* beyond bound   */

        size_t shift = (size_t)Ti;
        for (size_t d = 0; d <= k - shift; ++d) {
            coeff[d + shift] += H_old[d];
        }

        /* compute H_i from H_{i-1} -------------------------------------*/
        idx_t step = i + 1;
        for (size_t d = 0; d <= k; ++d) {
            H_new[d] = H_old[d];
            if (d >= (size_t)step)
                H_new[d] -= H_new[d - (size_t)step];
        }

        /* swap old and new ---------------------------------------------*/
        intmax_t *tmp = H_old;
        H_old        = H_new;
        H_new        = tmp;
    }

    /* collect zero coefficients ----------------------------------------*/
    seq_zero_len = 0;
    for (size_t d = 1; d <= k; ++d) {
        if (coeff[d] == 0) {
            seq_zero[seq_zero_len++] = (seq_t)d;
        }
    }
}

/* ------------------------------------------------------------------------*/
/* Grow the global bound until at least n zero indices are known.           */
static void enlarge_if_needed(idx_t n)
{
    if (seq_zero_len >= (size_t)n) return;

    do {
        max_k_global *= 2;
        build_sequence();
    } while (seq_zero_len < (size_t)n);
}

/* ------------------------------------------------------------------------*/
/* Public function ---------------------------------------------------------*/
seq_t a(idx_t n)
{
    enlarge_if_needed(n);
    /* n is 1‑based; seq_zero[0] holds a(1) */
    return seq_zero[n - 1];
}
