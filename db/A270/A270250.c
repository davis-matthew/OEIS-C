/*
 * A270250  Triangle read by rows: coefficients of the polynomial
 *          (Sum_{i=0..k} (Product_{j=0..i} (x+j)))/(x*(x+2)).
 * https://oeis.org/A270250
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   Triangle read by rows: coefficients of (∑_{i=0..k} ∏_{j=0..i}
 *            (x+j)) / (x*(x+2)) Rows have length k, coefficients are stored
 *            from the constant term upward
 * Range:     u8 n <= 15, u16 n <= 36, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <math.h>

static seq_t **rows = NULL;          /* rows[0] is row 1 */
static size_t rows_cap = 0;
static size_t max_computed = 0;      /* highest row number computed */

static seq_t *T_poly = NULL;        /* current polynomial T_k = ∏_{j=1..k, j≠2} (x+j) */
static size_t tlen = 0;             /* length of T_poly (equals current k) */

/* Ensure that rows 1..target_k have been computed. */
static void ensure_rows(size_t target_k)
{
    if (target_k <= max_computed)
        return;

    if (rows_cap < target_k) {
        seq_t **new_rows = (seq_t **)realloc(rows, target_k * sizeof(seq_t *));
        rows = new_rows;
        rows_cap = target_k;
    }

    if (max_computed == 0) {
        /* Compute row 1: Q_1 = 1 */
        rows[0] = (seq_t *)malloc(1 * sizeof(seq_t));
        rows[0][0] = (seq_t)1;
        max_computed = 1;

        /* Initialise T_poly for k = 2: (x + 1) */
        tlen = 2;
        T_poly = (seq_t *)malloc(tlen * sizeof(seq_t));
        T_poly[0] = (seq_t)1;   /* constant term */
        T_poly[1] = (seq_t)1;   /* coefficient of x */
    }

    for (size_t k = max_computed + 1; k <= target_k; ++k) {
        if (k == 2) {
            /* Q_2 = Q_1 + T_poly (T_poly is (x + 1)) */
            seq_t *row = (seq_t *)malloc(2 * sizeof(seq_t));
            row[0] = rows[0][0] + T_poly[0]; /* constant term: 1 + 1 = 2 */
            row[1] = T_poly[1];             /* x term: 1 */
            rows[1] = row;
            max_computed = 2;
            /* T_poly remains (x + 1) for the next iteration (k = 3) */
        } else {
            /* Multiply T_poly by (x + k) to obtain T_k */
            size_t oldlen = tlen;
            size_t newlen = oldlen + 1;
            seq_t *newT = (seq_t *)malloc(newlen * sizeof(seq_t));
            seq_t a = (seq_t)k;            /* coefficient of x in (x + k) */
            newT[0] = a * T_poly[0];
            for (size_t i = 1; i < newlen - 1; ++i) {
                newT[i] = a * T_poly[i] + T_poly[i - 1];
            }
            newT[newlen - 1] = T_poly[oldlen - 1];
            free(T_poly);
            T_poly = newT;
            tlen = newlen;

            /* Compute Q_k = Q_{k-1} + T_k */
            seq_t *prevRow = rows[k - 2];          /* row k-1, length k-1 */
            seq_t *row = (seq_t *)malloc(k * sizeof(seq_t));
            for (size_t i = 0; i < k - 1; ++i) {
                row[i] = prevRow[i] + T_poly[i];
            }
            row[k - 1] = T_poly[k - 1];            /* highest-degree term */
            rows[k - 1] = row;
            max_computed = k;
        }
    }
}

/* Return the n-th term (offset 1) of the sequence. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;            /* behavior below offset is unspecified */

    uint64_t nn = (uint64_t)n;
    /* Find smallest k with k*(k+1)/2 >= n */
    uint64_t k = (uint64_t)((sqrtl((long double)8.0L * (long double)nn + 1.0L) - 1.0L) / 2.0L);
    if (k * (k + 1) / 2 < nn)
        ++k;

    ensure_rows((size_t)k);

    uint64_t start = k * (k - 1) / 2 + 1;   /* first index in row k */
    uint64_t col   = nn - start;           /* 0‑based column in the row */

    return rows[k - 1][(size_t)col];
}
