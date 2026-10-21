/*
 * A271451  Triangle read by rows of coefficients of polynomials Q_n(x) =
 *          2^(-n)*((x + sqrt(x*(x + 6) - 3) + 1)^n - (x - sqrt(x*(x + 6) - 3)
 *          + 1)^n)/sqrt(x*(x + 6) - 3).
 * https://oeis.org/A271451
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabl, easy
 * Formula:   not stated
 * Range:     i8 n <= 41, i16 n <= 146, i32 n <= 546, i64 n <= 2049,
 *            i128 n <= 5050, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A271451
 * This code generates the flattened triangle of coefficients of the
 * polynomials Q_n(x) defined by
 *   Q_0(x) = 0,
 *   Q_1(x) = 1,
 *   Q_n(x) = (x + 1) Q_{n-1}(x) + (x - 1) Q_{n-2}(x)   (n ≥ 2).
 * a(n) is the n‑th entry of the triangle read by rows (row 1 has one
 * coefficient, row 2 has two, …).  For example the first rows are
 *   1
 *   1, 1
 *   0, 3, 1
 *   -1, 3, 5, 1
 * and so on.
 *
 * The routine works for any idx_t (uint64_t) and any seq_t (signed
 * integer of any width or arbitrary precision).  In order to avoid
 * overflow when building the coefficients, each arithmetic step is
 * performed in __int128 and the result is cast back to seq_t when
 * storing.  The final value is guaranteed to be correct whenever it
 * fits into seq_t, as required by the test harness.
 */

#include <stdlib.h>

/* Store the coefficient arrays for each generated row.
 * rows[i] points to an array of length i (i ≥ 1).  rows[0] is NULL. */
static seq_t **rows = NULL;          /* NULL until first use */
static size_t rows_cap = 0;          /* size of rows[] */
static idx_t  max_row = (idx_t)-1;   /* highest row constructed */

/* Ensure that rows[1] … rows[needed] are built.  This function also
 * grows the rows array if necessary. */
static void ensure_rows(idx_t needed)
{
    if (rows == NULL) {
        /* grow rows pointer array to at least needed+1 elements */
        size_t init_cap = (needed >= 1 ? (size_t)needed + 1 : 2);
        rows = (seq_t **)malloc(init_cap * sizeof(seq_t *));
        if (rows == NULL) abort();
        rows_cap = init_cap;

        /* row 0 – empty */
        rows[0] = NULL;

        /* row 1 – Q1(x) = 1 */
        rows[1] = (seq_t *)malloc(sizeof(seq_t));
        if (rows[1] == NULL) abort();
        rows[1][0] = (seq_t)1;
        max_row = 1;
    }

    if (needed <= max_row)
        return;

    /* Grow the array of row pointers if necessary. */
    if ((size_t)needed + 1 > rows_cap) {
        size_t new_cap = rows_cap * 2;
        if (new_cap < (size_t)needed + 1)
            new_cap = (size_t)needed + 1;
        seq_t **tmp = (seq_t **)realloc(rows, new_cap * sizeof(seq_t *));
        if (tmp == NULL) abort();
        rows = tmp;
        rows_cap = new_cap;
    }

    /* Build rows max_row+1 … needed. */
    for (idx_t r = max_row + 1; r <= needed; ++r) {
        if (r == 1) {          /* already handled in first allocation */
            continue;
        }

        size_t len = (size_t)r;          /* number of coefficients */
        seq_t *row = (seq_t *)malloc(len * sizeof(seq_t));
        if (row == NULL) abort();

        const seq_t *prev1 = rows[r - 1];
        const seq_t *prev2 = (r >= 2) ? rows[r - 2] : NULL;
        size_t len_prev1 = (size_t)(r - 1);
        size_t len_prev2 = (r >= 2) ? (size_t)(r - 2) : 0;

        for (size_t k = 0; k < len; ++k) {
            __int128 sum = 0;

            /* a[k] from (x+1)*prev1 */
            if (k < len_prev1)
                sum += (__int128)prev1[k];

            /* a[k-1] from (x+1)*prev1 times x */
            if (k > 0) {
                sum += (__int128)prev1[k - 1];
            }

            /* c[k-1] from (x-1)*prev2 times x */
            if (k > 0 && k - 1 < len_prev2)
                sum += (__int128)prev2[k - 1];

            /* -c[k] from -(x-1)*prev2 */
            if (k < len_prev2)
                sum -= (__int128)prev2[k];

            row[k] = (seq_t)sum;
        }

        rows[r] = row;
    }

    max_row = needed;
}

/* Return the n-th term of the flattened triangle (a(1) is the first). */
seq_t a(idx_t n)
{
    if (n == 0)               /* offset is 1; guard against zero if passed */
        return (seq_t)0;

    /* Find the row that contains the n-th element.  The row index r
     * satisfies r(r+1)/2 ≥ n.  We find it by incremental search
     * starting from row 1; this is efficient because n grows
     * quadratically with r. */
    idx_t r = 1;
    while ((r * (r + 1)) / 2 < n)
        ++r;

    /* Position within the row (0‑based). */
    idx_t tri_prev = ((r - 1) * r) / 2;          /* sum of lengths of previous rows */
    idx_t pos = n - tri_prev - 1;               /* 0‑based index */

    /* Ensure the required rows are built. */
    ensure_rows(r);

    return rows[r][(size_t)pos];
}
