/*
 * A318110  Triangle T(n,k) read by rows: coefficients of polynomials P_n(t)
 *          defined in Formula section.
 * https://oeis.org/A318110
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 20, u32 n <= 44, u64 n <= 119, u128 n <= 324,
 *            big any n
 * Helpers:   free_seq_arr
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A318110
   Triangle T(n,k) read by rows: coefficients of the polynomials P_n(t)
   defined by A(x,t) = Sum_{n>=0} P_n(t) x^n, satisfying
   A = x*t/(1-A) + ∂A/∂t,  A(0,t)=0,  ∂A/∂x(0,t)=1+t.
   The sequence a(n) is obtained by flattening this triangle row by row. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/free_seq_arr.h"

/* helpers that work both in C and C++ */
static seq_t* alloc_seq(size_t len)
{
#ifdef __cplusplus
    return new seq_t[len];
#else
    return (seq_t*)calloc(len, sizeof(seq_t));
#endif
}

/* storage for rows of the triangle and for the auxiliary polynomials
   B[n] = coefficient of x^n in (1-A)^{-1}  (row n). */
static seq_t **T_rows = NULL;   /* T_rows[n] -> array of length n+1 */
static seq_t **B_rows = NULL;   /* B_rows[n] -> array of length n+1 */
static size_t max_row_allocated = 0;  /* number of rows allocated */
static idx_t  last_built = (idx_t)-1;  /* last row that has been computed */

/* ensure that we have room for at least 'need' rows (indices 0 .. need-1) */
static void ensure_rows(size_t need)
{
    if (max_row_allocated >= need) return;
    size_t new_cap = need * 2 + 8;
    if (new_cap < 8) new_cap = 8;
    T_rows = (seq_t**)realloc(T_rows, new_cap * sizeof(seq_t*));
    B_rows = (seq_t**)realloc(B_rows, new_cap * sizeof(seq_t*));
    for (size_t i = max_row_allocated; i < new_cap; ++i) {
        T_rows[i] = NULL;
        B_rows[i] = NULL;
    }
    max_row_allocated = new_cap;
}

/* compute triangle rows up to r (inclusive) */
static void build_up_to(idx_t r)
{
    if (last_built == (idx_t)-1) {
        /* initialise row 0 */
        ensure_rows(1);
        T_rows[0] = alloc_seq(1);
        T_rows[0][0] = (seq_t)0;
        B_rows[0] = alloc_seq(1);
        B_rows[0][0] = (seq_t)1;
        last_built = 0;
    }

    if (last_built >= r) return;          /* already built */

    ensure_rows((size_t)r + 1);

    for (idx_t row = last_built + 1; row <= r; ++row) {
        size_t n = (size_t)row;
        size_t len = n + 1;

        /* ---------- compute P_n (triangle row) ---------- */
        seq_t *Bprev = B_rows[row-1];   /* B of previous row, length n */
        seq_t *h   = alloc_seq(len);
        for (size_t k = 1; k <= n; ++k) {
            if (k-1 < n) h[k] = Bprev[k-1];
            else h[k] = (seq_t)0;
        }
        seq_t *Pnext = alloc_seq(len);
        Pnext[n] = h[n];
        for (int k = (int)n-1; k >= 0; --k) {
            Pnext[k] = h[k] + (seq_t)(k+1) * Pnext[k+1];
        }
        free_seq_arr(h);
        T_rows[row] = Pnext;

        /* ---------- compute B_n (convolution) ---------- */
        seq_t *Bnext = alloc_seq(len);
        for (size_t j = 1; j <= n; ++j) {
            seq_t *Aj = T_rows[j];
            seq_t *Bj = B_rows[row - j];
            size_t Aj_len = j + 1;
            size_t Bj_len = (row - j) + 1;
            for (size_t a = 0; a < Aj_len; ++a) {
                const seq_t ca = Aj[a];
                for (size_t b = 0; b < Bj_len; ++b) {
                    Bnext[a+b] += ca * Bj[b];
                }
            }
        }
        B_rows[row] = Bnext;
    }
    last_built = r;
}

/*------------------------------------------------------------*/

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;

    /* find row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    size_t r = 0;
    while ((idx_t)(r+1) * ((idx_t)r + 2ULL) / 2 <= n) ++r;

    build_up_to((idx_t)r);

    idx_t prev_total = (idx_t)r * ((idx_t)r + 1ULL) / 2;
    size_t k = (size_t)(n - prev_total);
    return T_rows[r][k];
}
