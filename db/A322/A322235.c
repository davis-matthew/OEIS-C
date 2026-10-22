/*
 * A322235  Triangle, read by rows, each row n being defined by g.f.
 *          Product_{k=1..n} (k + x + k*x^2), for n >= 0.
 * https://oeis.org/A322235
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 51, u32 n <= 125, u64 n <= 303,
 *            u128 n <= 859, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) from OEIS A322235.
 * a(n) is the flattened triangle of coefficients of
 *   P_r(x) = ∏_{k=1..r} (k + x + k*x^2)
 * where r satisfies r^2 ≤ n < (r+1)^2, and k = n - r^2.
 * The function keeps a static cache of all rows up to the
 * largest r requested.
 */

#include <stddef.h>   /* size_t, NULL */
#include <stdlib.h>   /* malloc, calloc, realloc */

seq_t a(idx_t n)
{
    /* Determine row r and position k within row */
    idx_t r = 0;
    while ((r + 1) * (r + 1) <= n) {
        r++;
    }
    idx_t k = n - r * r;

    /* Static cache of rows */
    static seq_t **rows = NULL;      /* rows[0] … rows[max_cached]   */
    static size_t *rowlen = NULL;    /* length of each row           */
    static idx_t max_cached = 0;     /* largest row that has been built */

    /* Allocate initial row 0 if cache empty */
    if (rows == NULL) {
        rows = (seq_t **)malloc(sizeof(seq_t *));
        rowlen = (size_t *)malloc(sizeof(size_t));
        rows[0] = (seq_t *)calloc(1, sizeof(seq_t));
        rowlen[0] = 1;
        rows[0][0] = (seq_t)1;   /* coefficient of x^0 in empty product */
        max_cached = 0;
    }

    /* Build rows up to r if necessary */
    if (r > max_cached) {
        size_t new_cap = (size_t)(r + 1);
        rows = (seq_t **)realloc(rows, new_cap * sizeof(seq_t *));
        rowlen = (size_t *)realloc(rowlen, new_cap * sizeof(size_t));

        for (idx_t cur = max_cached + 1; cur <= r; ++cur) {
            size_t cur_len = 2 * (size_t)cur + 1;
            rows[cur] = (seq_t *)calloc(cur_len, sizeof(seq_t));
            rowlen[cur] = cur_len;

            /*  The convolution of previous row with [cur, 1, cur]  */
            seq_t *prev = rows[cur - 1];
            size_t prev_len = rowlen[cur - 1];
            seq_t *cur_row = rows[cur];
            seq_t mult = (seq_t)cur;        /* constant and x^2 coefficient */

            for (size_t i = 0; i < prev_len; ++i) {
                seq_t val = prev[i];
                if (val == (seq_t)0) continue;
                cur_row[i]     += val * mult;   /* constant term   */
                cur_row[i + 1] += val;         /* x term          */
                cur_row[i + 2] += val * mult;   /* x^2 term        */
            }
        }
        max_cached = r;
    }

    return rows[r][k];
}
