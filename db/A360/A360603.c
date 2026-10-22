/*
 * A360603  Triangle read by rows. T(n, k) = A360604(n, k) * A001187(k) for 0
 *          <= k <= n.
 * https://oeis.org/A360603
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 33, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 *  OEIS A360603 – flattened triangle read by rows.
 *  a(n) = T(r, c) where r and c satisfy
 *      n = r*(r+1)/2 + c     (0 ≤ c ≤ r).
 *  T(r, c) =
 *      0                                 if r > 0 and c = 0,
 *      A001187(r)                        if c = r,
 *      2^((r-c)*(r-c-1)/2) * binomial(r-1, c-1) * A001187(c)  otherwise.
 *
 *  The implementation computes the triangle row by row, caching the
 *  values of A001187(k) (connected labeled graphs).  For each new row
 *  n it uses the well‑known recurrence
 *      A001187(n) = 2^(n*(n-1)/2) -
 *                   Σ_{k=1}^{n-1} binomial(n-1,k-1) * A001187(k) *
 *                                   2^((n-k)*(n-k-1)/2).
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "../utils/binomial.h"   /* seq_t binomial(idx_t n, idx_t k); */
#include "../utils/pow2.h"       /* seq_t pow2(idx_t exp); */

/* cached values of A001187(k) */
static seq_t *conn = NULL;          /* allocated as an array of seq_t */
static size_t conn_cap = 0;         /* current capacity of conn */
static size_t conn_len = 0;         /* number of computed values (conn_len-1 is max n computed) */

/* Expand conn array and compute missing consecutive values up to “needed-1”. */
static void
ensure_conn(size_t needed)
{
    if (needed <= conn_len)
        return;

    if (needed > conn_cap) {
        size_t new_cap = (conn_cap == 0) ? 64 : conn_cap * 2;
        while (new_cap < needed)
            new_cap <<= 1;
        seq_t *new_arr = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (conn) {
            memcpy(new_arr, conn, conn_len * sizeof(seq_t));
        }
        memset(new_arr + conn_len, 0, (new_cap - conn_len) * sizeof(seq_t));
        free(conn);
        conn = new_arr;
        conn_cap = new_cap;
    }

    /* compute from conn_len up to needed-1 */
    for (size_t n = conn_len; n < needed; ++n) {
        /* total = 2^(n*(n-1)/2) */
        seq_t total = pow2((idx_t)n * (n - 1) / 2);

        seq_t sum = 0;
        for (size_t k = 1; k < n; ++k) {
            seq_t term = binomial(n - 1, k - 1);   /* C(n-1, k-1) */
            term = term * conn[k];
            idx_t exp = (idx_t)(n - k) * (n - k - 1) / 2;
            term = term * pow2(exp);
            sum = sum + term;
        }

        conn[n] = total - sum;
    }

    conn_len = needed;
}

/* ------------------------------------------------------------------ */
/* Main sequence function */

seq_t
a(idx_t n)
{
    /* Convert linear index n to row r and column c */
    idx_t r = 0;
    while (true) {
        idx_t pre = r * (r + 1) / 2;            /* elements before row r */
        idx_t next_pre = (r + 1) * (r + 2) / 2; /* elements before row r+1 */
        if (n < next_pre) {
            idx_t c = n - pre;                  /* column within the row */

            /* Handle special cases */
            if (r == 0)          /* T(0,0) = 1 */
                return (seq_t)1;
            if (c == 0)          /* first element of rows n ≥ 1 is 0 */
                return (seq_t)0;
            if (c == r) {        /* last element of row r */
                ensure_conn(r + 1);
                return conn[r];
            }

            /* General case: T(r,c) for 0 < c < r */
            ensure_conn(c + 1);           /* make sure conn[c] is available */

            seq_t term = binomial(r - 1, c - 1);
            term = term * conn[c];
            idx_t exp = (idx_t)(r - c) * (r - c - 1) / 2;
            term = term * pow2(exp);
            return term;
        }
        ++r;
    }
}
