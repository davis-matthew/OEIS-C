/*
 * A289546  Triangle read by rows. T(n,k) is the number of flags in an n
 *          dimensional vector space over GF(2) that have length exactly k, n
 *          >= 0, 0 <= k <= n.
 * https://oeis.org/A289546
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 23, u32 n <= 40, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   flat_to_rc, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A289546
    a(n) is the triangular array T(n,k) read by rows.
    Recurrence:
      T(0,0) = 1,
      T(n,0) = 0  (n>0),    T(n,1) = 1  (n>0)
      T(n,k) = Σ_{d=k-1}^{n-1} GaussianBinom(n,d) * T(d,k-1)   (k≥2)
    The Gaussian binomial coefficients are for q = 2:
      G(n,0)=G(n,n)=1
      G(n,k) = G(n-1,k-1) + 2^k * G(n-1,k)
*/

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/seq_mul.h"
#include "../utils/flat_to_rc.h"

/* static storage for Gaussian binomials and flags */
static seq_t **gbinom_rows = NULL;  /* gbinom_rows[n][k] = GaussianBinom(n,k) */
static size_t gbinom_rows_cap = 0;  /* number of rows prepared */

static seq_t *pow2 = NULL;   /* pow2[i] = 2^i in seq_t */
static idx_t pow2_cap = 0;   /* number of entries stored */

static seq_t **T_rows = NULL;   /* T_rows[n][k] = T(n,k) */
static size_t T_rows_cap = 0;   /* number of rows prepared */

/* ensure powers of two up to ‘upto’ are computed */
static void ensure_pow2(idx_t upto)
{
    if (pow2 == NULL) {
        pow2 = (seq_t*)calloc(1, sizeof(seq_t));
        pow2[0] = (seq_t)1;
        pow2_cap = 1;
    }
    for (idx_t i = pow2_cap; i <= upto; ++i) {
        pow2 = (seq_t*)realloc(pow2, (size_t)(i + 1) * sizeof(seq_t));
        pow2[i] = seq_mul(pow2[i - 1], (seq_t)2);
    }
    pow2_cap = upto + 1;
}

/* ensure Gaussian binomial tables up to ‘upto’ are computed */
static void ensure_gbinom(idx_t upto)
{
    if (gbinom_rows == NULL) {
        gbinom_rows = (seq_t**)calloc(1, sizeof(seq_t*));
        gbinom_rows[0] = (seq_t*)calloc(1, sizeof(seq_t));
        gbinom_rows[0][0] = (seq_t)1;
        gbinom_rows_cap = 1;
    }
    ensure_pow2(upto);
    for (idx_t n = gbinom_rows_cap; n <= upto; ++n) {
        seq_t *row = (seq_t*)calloc((size_t)n + 1, sizeof(seq_t));
        row[0] = (seq_t)1;
        row[n] = (seq_t)1;
        for (idx_t k = 1; k < n; ++k) {
            seq_t left  = gbinom_rows[n - 1][k - 1];
            seq_t right = gbinom_rows[n - 1][k];
            seq_t mult  = seq_mul(right, pow2[k]);      /* right * 2^k */
            row[k] = left + mult;
        }
        gbinom_rows[n] = row;
    }
    gbinom_rows_cap = upto + 1;
}

/* ensure flag table up to ‘upto’ is computed */
static void ensure_T(idx_t upto)
{
    if (T_rows == NULL) {
        T_rows = (seq_t**)calloc(1, sizeof(seq_t*));
        T_rows[0] = (seq_t*)calloc(1, sizeof(seq_t));
        T_rows[0][0] = (seq_t)1;
        T_rows_cap = 1;
    }
    ensure_gbinom(upto);
    for (idx_t n = T_rows_cap; n <= upto; ++n) {
        seq_t *row = (seq_t*)calloc((size_t)n + 1, sizeof(seq_t));
        if (n == 0) {
            row[0] = (seq_t)1;
        } else {
            row[0] = (seq_t)0;      /* T(n,0)=0 for n>0 */
            row[1] = (seq_t)1;      /* T(n,1)=1 for n>0 */
            for (idx_t k = 2; k <= n; ++k) {
                seq_t sum = (seq_t)0;
                for (idx_t d = k - 1; d <= n - 1; ++d) {
                    seq_t prod = seq_mul(gbinom_rows[n][d], T_rows[d][k - 1]);
                    sum += prod;
                }
                row[k] = sum;
            }
        }
        T_rows[n] = row;
    }
    T_rows_cap = upto + 1;
}

/* user requested function */
seq_t a(idx_t n)
{
    idx_t row, col;
    flat_to_rc(n, &row, &col);
    if (col > row) return (seq_t)0;  /* safety */
    ensure_T(row);
    return T_rows[row][col];
}
