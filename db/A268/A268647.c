/*
 * A268647  G.f.: C(x,y)^2 - S(x,y)^2 = Sum_{n>=0} x^(2*n)*y/[Sum_{k=0..2*n+1}
 *          T(n,k)*y^k], where C(x,y) = Sum_{n>=0} x^(2*n) /
 *          Product_{k=1..2*n} (k + y) and S(x,y) = Sum_{n>=0} x^(2*n+1) /
 *          Product_{k=1..2*n+1} (k + y).
 * https://oeis.org/A268647
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = T(r,k), where r = max{t | t(t+1)≤n} and k = n – r(r+1).
 *            T(r,k) is the coefficient of y^k in (r + y)∏_{i=1}^{2r}(i + y)
 * Range:     u8 n <= 11, u16 n <= 19, u32 n <= 42, u64 n <= 109,
 *            u128 n <= 305, big any n
 * Helpers:   max_r, poly_mul_by_linear
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Standard headers  */
#include <stdlib.h>
#include "../utils/max_r.h"
#include "../utils/poly_mul_by_linear.h"

/*  Maximum index of a precomputed row.  */
static idx_t cached_rows = 0;          /* number of rows stored (max index + 1) */
static seq_t **row_cache = NULL;       /* array of pointers to rows */

/*  Build the coefficient array for row r of the triangle T.  */
static seq_t *build_row(idx_t r)
{
    /* Polynomial P = ∏_{i=1}^{2r} (i + y), degree 2r.  */
    seq_t *P = (seq_t *)calloc(1, sizeof(seq_t));
    if (!P) return NULL;
    P[0] = (seq_t)1;
    size_t deg = 0;
    for (idx_t i = 1; i <= 2 * r; ++i) {
        seq_t *Q = poly_mul_by_linear(P, deg, (unsigned int)i);
        free(P);
        if (!Q) return NULL;
        P = Q;
        ++deg;          /* degree increased by 1 */
    }

    /* Compute T(r,k) = (r + y) * P.  */
    size_t lenT = 2 * r + 2;                /* indices 0 … 2r+1 */
    seq_t *T = (seq_t *)malloc(lenT * sizeof(seq_t));
    if (!T) {
        free(P);
        return NULL;
    }

    seq_t sr = (seq_t)r;
    T[0] = sr * P[0];
    for (idx_t k = 1; k <= 2 * r; ++k) {
        T[k] = sr * P[(size_t)k] + P[(size_t)k - 1];
    }
    T[2 * r + 1] = P[2 * r];
    free(P);
    return T;
}

/*  Ensure that rows 0 … r are built in the cache.  */
static void ensure_rows(idx_t r)
{
    if (cached_rows > r) return;
    size_t new_cap = cached_rows ? cached_rows * 2 : 8;
    while (new_cap <= r) new_cap <<= 1;
    if (new_cap > cached_rows) {
        seq_t **tmp = (seq_t **)realloc(row_cache, new_cap * sizeof(seq_t *));
        if (!tmp) exit(1);            /* fatal error */
        row_cache = tmp;
    }
    for (idx_t i = cached_rows; i <= r; ++i) {
        row_cache[i] = build_row(i);
        if (!row_cache[i]) exit(1);
    }
    cached_rows = r + 1;
}

/*  Main sequence function required by the framework.  */
seq_t a(idx_t n)
{
    idx_t r = max_r(n);
    idx_t k = n - r * (r + 1);

    ensure_rows(r);
    return row_cache[r][k];
}
