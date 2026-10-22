/*
 * A399848  Triangle read by rows: T(n,k) = n! * [x^n] log(x+1)^k/(k*(1 -
 *          log(x+1))^(k+1)), with 1 <= k <= n.
 * https://oeis.org/A399848
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(n,k) = n! * [x^n] log(x+1)^k/(k*(1 - log(x+1))^(k+1)) a(n) is
 *            the term read from the triangle by rows
 * Range:     u8 n <= 11, u16 n <= 30, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   binomial, get_row_col
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/binomial.h"   /* binomial(idx_t n, idx_t k) */
#include "../utils/get_row_col.h"

static seq_t **c_tbl = NULL;          /* unsigned Stirling numbers of first kind c(n,k) */
static idx_t  c_cap = 0;
static seq_t *fac  = NULL;           /* factorial table fac[n] = n!          */
static idx_t  fac_cap = 0;

/* Ensure tables up to n are allocated and filled. */
static void ensure_c_up_to(idx_t n)
{
    if (n < c_cap) return;
    idx_t new_cap = c_cap ? c_cap * 2 : 128;
    if (new_cap < n + 1) new_cap = n + 1;

    seq_t **new_tbl = (seq_t **)malloc(new_cap * sizeof(seq_t *));
    for (idx_t i = 0; i < c_cap; ++i) new_tbl[i] = c_tbl[i];
    for (idx_t i = c_cap; i < new_cap; ++i) new_tbl[i] = NULL;
    free(c_tbl);
    c_tbl = new_tbl;

    for (idx_t i = c_cap; i <= n; ++i) {
        c_tbl[i] = (seq_t *)calloc(i + 1, sizeof(seq_t));
        if (i == 0) {
            c_tbl[0][0] = (seq_t)1;
        } else {
            c_tbl[i][0] = (seq_t)0;
            for (idx_t k = 1; k < i; ++k) {
                seq_t val1 = c_tbl[i - 1][k - 1];
                seq_t val2 = c_tbl[i - 1][k];
                seq_t mult = (seq_t)(i - 1) * val2;
                c_tbl[i][k] = val1 + mult;
            }
            c_tbl[i][i] = (seq_t)1;
        }
    }
    c_cap = n + 1;
}

static void ensure_fac_up_to(idx_t n)
{
    if (n < fac_cap) return;
    idx_t new_cap = fac_cap ? fac_cap * 2 : 128;
    if (new_cap < n + 1) new_cap = n + 1;

    seq_t *new_fac = (seq_t *)realloc(fac, new_cap * sizeof(seq_t));
    for (idx_t i = fac_cap; i <= n; ++i) {
        if (i == 0) {
            new_fac[0] = (seq_t)1;
        } else {
            new_fac[i] = new_fac[i - 1] * (seq_t)i;
        }
    }
    fac = new_fac;
    fac_cap = n + 1;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* offset is 1, but guard anyway */

    idx_t r, k;
    get_row_col(n, &r, &k);

    ensure_fac_up_to(r);
    ensure_c_up_to(r);

    seq_t pos = (seq_t)0;   /* sum of terms with even parity of (r‑p) */
    seq_t neg = (seq_t)0;   /* sum of terms with odd parity           */

    for (idx_t p = k; p <= r; ++p) {
        seq_t term = binomial(p, k);
        term = term * c_tbl[r][p];
        term = term * fac[p];
        if (((r - p) & 1) == 0) {
            pos += term;
        } else {
            neg += term;
        }
    }

    seq_t sum = pos;
    sum -= neg;                 /* guaranteed non‑negative for all n */
    seq_t result = sum / (seq_t)k;
    return result;
}
