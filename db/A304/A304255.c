/*
 * A304255  Triangle read by rows: T(0,0) = 1; T(n,k) = 6*T(n-1,k) +
 *          T(n-2,k-1) for k = 0..floor(n/2); T(n,k)=0 for n or k < 0.
 * https://oeis.org/A304255
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  tabf, nonn, easy
 * Formula:   a(n) is the n-th element of the triangle read by rows where
 *            T(0,0)=1 and, for 0≤k≤floor(n/2), T(n,k)=6*T(n‑1,k)+T(n‑2,k‑1)
 * Range:     u8 n <= 5, u16 n <= 15, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t **T = NULL;          /* array of pointers to rows */
static idx_t rows_cap = 0;        /* allocated number of rows */
static idx_t max_row = 0;         /* highest computed row index + 1 */

/* Ensure that rows 0 … r exist and are fully filled. */
static void ensure_row(idx_t r)
{
    if (r < max_row) return;           /* already computed */
    /* grow rows array to hold up to r */
    if (r + 1 > rows_cap) {
        idx_t new_cap = rows_cap ? rows_cap * 2 : 16;
        if (new_cap <= r + 1) new_cap = r + 1;
        T = (seq_t **)realloc(T, new_cap * sizeof(seq_t *));
        rows_cap = new_cap;
    }
    while (max_row <= r) {
        idx_t cur = max_row;           /* row index to compute now */
        idx_t len = cur / 2 + 1;
        T[cur] = (seq_t *)calloc(len, sizeof(seq_t));
        if (cur == 0) {                /* base */
            T[0][0] = (seq_t)1;
        } else {
            /* For each k in this row, compute using the recurrence. */
            for (idx_t k = 0; k < len; ++k) {
                seq_t term1 = 0, term2 = 0;
                if (k <= (cur - 1) / 2)   /* T[cur-1][k] exists */
                    term1 = ((seq_t)6) * T[cur - 1][k];
                if (cur >= 2 && k >= 1 &&
                    (k - 1) <= (cur - 2) / 2)
                    term2 = T[cur - 2][k - 1];
                T[cur][k] = term1 + term2;
            }
        }
        ++max_row;
    }
}

/* Return the n-th term of the flattened triangle. */
seq_t a(idx_t n)
{
    /* Find the row r such that cumulative count <= n < cumulative count + row_len */
    idx_t cum = 0, r = 0;
    while (1) {
        idx_t len = r / 2 + 1;
        if (cum + len > n) break;
        cum += len;
        ++r;
    }
    idx_t k = n - cum;          /* column within row r */
    ensure_row(r);              /* make sure row r is computed */
    return T[r][k];
}
