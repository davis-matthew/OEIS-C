/*
 * A248847  Triangle read by rows: T(m,n) = number of ways of distributing n
 *          distinguishable balls into m distinguishable bins of size 5 where
 *          empty bins are permitted (m >= 1, 1 <= n <= 5m).
 * https://oeis.org/A248847
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) is read row‑by‑row from the triangle T(m,k) where T(m,0)=1
 *            and T(m,k)=∑_{c=0}^{min(5,k)} T(m-1,k-c)·C(k,c) for m≥1, k≥0
 *            (C(k,c) is binomial(k,c)). The index n is converted to row m and
 *            position k by m = smallest positive integer with 5*m*(m+1)/2 ≥ n
 *            k = n - 5*(m-1)*m/2 (1 ≤ k ≤ 5m)
 * Range:     u8 n <= 12, u16 n <= 25, u32 n <= 47, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Standard headers */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Utility for binomial coefficients */
#include "../utils/binom.h"

/* Global cache of computed rows.
   rows[m] points to an array of length 5*m+1 holding T(m,k) for k=0..5m. */
static seq_t **rows = NULL;
static size_t rows_len = 0;      /* number of rows currently stored (0 means none) */

/* Ensure that rows 0..m are computed, allocating storage as needed. */
static void compute_rows_up_to(idx_t m)
{
    if ((size_t)m < rows_len) return;          /* already available */

    /* Grow the rows array to hold indices up to m. */
    size_t new_len = (size_t)m + 1;
    seq_t **new_rows = (seq_t **)(realloc(rows, new_len * sizeof(seq_t *)));
    if (!new_rows) abort();                    /* allocation failed */
    rows = new_rows;
    rows_len = new_len;

    /* Row 0: only 0 balls can be distributed among 0 bins. */
    if (!rows[0]) {
        rows[0] = (seq_t *)(malloc(sizeof(seq_t)));
        if (!rows[0]) abort();
        rows[0][0] = (seq_t)1;
    }

    /* Compute any missing rows up to m. */
    for (size_t cur = 1; cur <= (size_t)m; ++cur) {
        if (rows[cur]) continue;               /* already computed */

        idx_t kmax = 5 * (idx_t)cur;
        size_t cur_size = (size_t)kmax + 1;
        seq_t *cur_row = (seq_t *)calloc(cur_size, sizeof(seq_t));
        if (!cur_row) abort();

        seq_t *prev_row = rows[cur - 1];
        idx_t prev_kmax = 5 * (idx_t)(cur - 1);

        for (idx_t k = 0; k <= kmax; ++k) {
            seq_t sum = (seq_t)0;
            idx_t cmax = (k < 5) ? k : 5;
            for (idx_t c = 0; c <= cmax; ++c) {
                idx_t rem = k - c;
                if (rem > prev_kmax) continue;
                seq_t pr = prev_row[rem];
                if (!pr) continue;
                seq_t bin = binom((idx_t)k, (idx_t)c);
                sum += pr * bin;
            }
            cur_row[k] = sum;
        }
        rows[cur] = cur_row;
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;            /* undefined for n<1, return 0 */

    /* Find the minimal m with 5*m*(m+1)/2 ≥ n. */
    idx_t m = 1;
    while ((unsigned __int128)5 * (unsigned __int128)m *
           ((unsigned __int128)m + 1) / 2 < n) {
        ++m;
    }

    /* Compute how many terms precede this row. */
    idx_t cum_before =
        (idx_t)(((unsigned __int128)5 * (unsigned __int128)(m - 1) *
                 (unsigned __int128)m) / 2);

    /* Position within the row (1‑based). */
    idx_t k = n - cum_before;          /* 1 ≤ k ≤ 5m */

    /* Ensure the required rows are computed. */
    compute_rows_up_to(m);

    /* Return the requested entry. */
    return rows[m][k];
}
