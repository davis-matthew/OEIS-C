/*
 * A368151  Triangular array T(n,k), read by rows: coefficients of strong
 *          divisibility sequence of polynomials p(1,x) = 1, p(2,x) = 1 + 3x,
 *          p(n,x) = u*p(n-1,x) + v*p(n-2,x) for n >=3, where u = p(2,x), v =
 *          2 - x^2.
 * https://oeis.org/A368151
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the n‑th coefficient obtained by reading the triangular
 *            array T(n,k) of the polynomials p(n,x) by rows. The polynomials
 *            satisfy p(1,x) = 1, p(2,x) = 1 + 3x, p(n,x) = (1+3x) · p(n-1,x)
 *            + (2-x²) · p(n-2,x) (n ≥ 3). The coefficient T(n,k) (1 ≤ k ≤ n)
 *            is the coefficient of x^{k-1} in p(n,x)....
 * Range:     u8 n <= 17, u16 n <= 49, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* find the row r such that sum_{i=1}^{r-1} i < n ≤ sum_{i=1}^{r} i */
    idx_t r = 0;
    idx_t cum = 0;
    while (cum < n) {
        r++;
        cum += r;
    }
    /* column index within row r (1‑based) */
    idx_t prev_sum = cum - r;
    idx_t col = n - prev_sum;            /* 1‑based */

    /* static storage of rows already computed */
    static seq_t **rows = NULL;           /* array of pointers to rows */
    static size_t rows_cap = 0;           /* capacity of rows */
    static idx_t rows_count = 0;          /* number of rows stored (≥0) */

    /* expand storage if necessary */
    if (rows_count < r) {
        size_t need = (size_t)r;
        if (rows_cap < need) {
            size_t new_cap = rows_cap ? (rows_cap << 1) : 1;
            if (new_cap < need) new_cap = need;
            seq_t **tmp = (seq_t**)realloc(rows, sizeof(seq_t*) * new_cap);
            if (!tmp) abort();          /* allocation failed */
            rows = tmp;
            rows_cap = new_cap;
        }
        /* compute rows up to r */
        while (rows_count < r) {
            idx_t cur = rows_count + 1;          /* row number (1‑based) */
            idx_t len = cur;                     /* number of elements */
            seq_t *row = (seq_t*)malloc(sizeof(seq_t) * (size_t)len);
            if (!row) abort();                  /* allocation failed */

            if (cur == 1) {
                row[0] = (seq_t)1;
            } else if (cur == 2) {
                row[0] = (seq_t)1;
                row[1] = (seq_t)3;
            } else {
                seq_t *prev1 = rows[rows_count - 1];   /* row n-1 */
                seq_t *prev2 = rows[rows_count - 2];   /* row n-2 */
                for (idx_t k = 1; k <= cur; ++k) {
                    seq_t val = 0;
                    /* T(n-1,k) */
                    if (k <= cur - 1)
                        val += prev1[k - 1];
                    /* 3·T(n-1,k-1) */
                    if (k >= 2 && k - 1 <= cur - 1)
                        val += 3 * prev1[k - 2];
                    /* 2·T(n-2,k) */
                    if (k <= cur - 2)
                        val += 2 * prev2[k - 1];
                    /* –T(n-2,k-2) */
                    if (k >= 3 && k - 2 <= cur - 2)
                        val -= prev2[k - 3];
                    row[k - 1] = val;
                }
            }
            rows[rows_count] = row;
            rows_count++;
        }
    }
    /* return the stored value */
    return rows[r - 1][col - 1];
}
