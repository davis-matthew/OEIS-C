/*
 * A291137  Square array A(n,k), n >= 0, k >= 0, read by antidiagonals, where
 *          column k is the expansion of inverse of k-th cyclotomic
 *          polynomial.
 * https://oeis.org/A291137
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   not stated
 * Range:     i8 n <= 104, i16 n <= 104, i32 n <= 104, i64 n <= 104,
 *            i128 n <= 104, big any n
 * Helpers:   mu_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A291137
    a(n) = coefficient of x^i in the series expansion of 1/Φ_j(x),
    where (i,j) are obtained from n by the antidiagonal mapping:
        s = ⌊(√(8n+1)-1)/2⌋ ,   i = n - s(s+1)/2,   j = s - i
    and Φ_j(x) is the j‑th cyclotomic polynomial.
    For j = 0 the column contains a single 1 at i = 0.
    For j = 1 the column consists of all -1 (period 1).
    For j ≥ 2 the column is periodic of period j and is given by
        1/Φ_j(x) = ∏_{d|j} 1/(1 - x^(j/d))^μ(d),
    where μ is the Möbius function.  The coefficient at index i is taken
    modulo j.
*/

#include <stdlib.h>
#include <math.h>
#include "../utils/mu_of.h"

static seq_t col0[1] = { (seq_t)1 };   /* column 0 */
static seq_t col1[1] = { (seq_t)-1 };  /* column 1 */

static seq_t **cols = NULL;   /* pointers to computed columns j ≥ 2 */
static size_t cols_cap = 0;   /* current capacity of cols */

/* ------------------------------------------------------------------ */
/* Compute the j‑th column (j ≥ 2).  The column has j coefficients
   for indices 0 … j-1, representing the periodic sequence of
   coefficients of 1/Φ_j(x).  */
static seq_t *compute_col(idx_t j)
{
    seq_t *c = (seq_t*)calloc((size_t)j, sizeof(seq_t));
    if (!c) return NULL;   /* undefined behaviour if allocation fails */
    c[0] = 1;              /* 1/(1−x^m) starts with 1 */

    for (idx_t d = 1; d * d <= j; ++d) {
        if (j % d != 0) continue;

        /* first divisor d */
        int mu = mu_of(d);
        if (mu != 0) {
            idx_t m = j / d;
            if (m < j) {
                if (mu == 1) {                    /* multiply by 1/(1−x^m) */
                    for (idx_t n = m; n < j; ++n)
                        c[n] += c[n - m];
                } else /* mu == -1 */ {             /* multiply by (1−x^m) */
                    for (idx_t n = j - 1; ; --n) {
                        c[n] -= c[n - m];
                        if (n == m) break;
                    }
                }
            }
        }

        /* paired divisor d2 = j/d, distinct from d */
        idx_t d2 = j / d;
        if (d2 != d) {
            int mu2 = mu_of(d2);
            if (mu2 != 0) {
                idx_t m2 = j / d2;   /* equals d */
                if (m2 < j) {
                    if (mu2 == 1) {
                        for (idx_t n = m2; n < j; ++n)
                            c[n] += c[n - m2];
                    } else { /* mu2 == -1 */
                        for (idx_t n = j - 1; ; --n) {
                            c[n] -= c[n - m2];
                            if (n == m2) break;
                        }
                    }
                }
            }
        }
    }
    return c;
}

/* ------------------------------------------------------------------ */
/* Return a pointer to the j‑th column, allocating it on first use. */
static seq_t *get_col(idx_t j)
{
    if (j == 0) return col0;
    if (j == 1) return col1;

    if (j >= cols_cap) {
        size_t new_cap = cols_cap ? cols_cap * 2 : 16;
        while (new_cap <= j) new_cap *= 2;
        seq_t **tmp = (seq_t**)realloc(cols, new_cap * sizeof(seq_t*));
        if (!tmp) return NULL;  /* allocation failure: undefined behaviour */
        for (size_t k = cols_cap; k < new_cap; ++k) tmp[k] = NULL;
        cols = tmp;
        cols_cap = new_cap;
    }
    if (!cols[j]) {
        cols[j] = compute_col(j);
    }
    return cols[j];
}

/* ------------------------------------------------------------------ */
/* Public sequence function required by the build system. */
seq_t a(idx_t n)
{
    /* antidiagonal mapping */
    long double d = sqrtl(8.0L * (long double)n + 1.0L);
    idx_t s = (idx_t)((d - 1.0L) / 2.0L);
    idx_t tri = s * (s + 1) / 2;
    idx_t offset = n - tri;

    idx_t i = offset;              /* row index */
    idx_t j = s - offset;          /* column index */

    if (j == 0) {                  /* column 0: only 1 at i=0 */
        return (i == 0) ? (seq_t)1 : (seq_t)0;
    }

    seq_t *col = get_col(j);
    if (!col) return (seq_t)0;     /* safe return on allocation failure */

    idx_t idx = i % j;             /* periodicity of the column */
    return col[idx];
}
