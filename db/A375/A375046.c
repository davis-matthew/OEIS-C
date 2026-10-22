/*
 * A375046  Irregular triangular array T; row n shows the coefficients of the
 *          (n-1)-st polynomial in the obverse convolution s(x)**t(x), where
 *          s(x) = 2^n and t(x) = x+2. See Comments.
 * https://oeis.org/A375046
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   * Let A[0][0] = 3, A[0][1] = 1. * For k ≥ 1 and 0 ≤ j ≤ k+1: *
 *            A[k][j] = (2^k + 2) * A[k-1][j] + A[k-1][j-1] * (where A[k-1][j]
 *            is taken as 0 if j > k). * The sequence a(n) is obtained by
 *            reading the rows of A * row‑wise: a(1)=A[0][0], a(2)=A[0][1],
 *            a(3)=A[1][0], … * For a given n, find the uniqu...
 * Range:     u8 n <= 9, u16 n <= 20, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/pow2.h"

typedef uint64_t idx_t;   /* will be replaced by the build system */
typedef unsigned __int128 u128; /* for internal 128‑bit arithmetic */

static seq_t **rows = NULL;      /* rows[k] points to array of length k+2 */
static size_t rows_cap = 0;      /* allocated capacity of rows[] */
static size_t rows_len = 0;      /* number of rows actually computed */

/* Ensure that rows[0]..rows[k] are computed */
static void ensure_rows_up_to(size_t k)
{
    size_t i;
    seq_t *prev_row;
    seq_t factor;
    for (i = rows_len; i <= k; ++i)
    {
        size_t len = i + 2;          /* row length */
        seq_t *row = (seq_t *)malloc(len * sizeof(seq_t));
        if (!row) return;           /* OOM, return without incrementing heap */

        if (i == 0)                  /* base row: 3, 1 */
        {
            row[0] = (seq_t)3;
            row[1] = (seq_t)1;
        }
        else
        {
            prev_row = rows[i - 1];
            factor = pow2((idx_t)i) + (seq_t)2; /* 2^i + 2 */

            /* first coefficient */
            row[0] = factor * prev_row[0];

            /* middle coefficients */
            size_t prev_len = (i - 1) + 2; /* = i + 1 */
            size_t j;
            for (j = 1; j < len; ++j)
            {
                seq_t t1 = (j < prev_len) ? (factor * prev_row[j]) : (seq_t)0;
                seq_t t2 = prev_row[j - 1];
                row[j] = t1 + t2;
            }
        }

        /* make sure we have enough slots in the rows array */
        if (rows_len >= rows_cap)
        {
            size_t new_cap = (rows_cap == 0) ? 8 : rows_cap * 2;
            seq_t **tmp = (seq_t **)realloc(rows, new_cap * sizeof(seq_t *));
            if (!tmp) return;          /* OOM, abort extension */
            rows = tmp;
            rows_cap = new_cap;
        }

        rows[rows_len++] = row;
    }
}

/* Find the row r (1‑based) containing the nth term */
static idx_t row_of_n(idx_t n)
{
    idx_t low = 1, high = 1;
    /* expand high until it covers n */
    while ((u128)high * (high + 3) / 2 < n)
        high <<= 1;

    /* binary search */
    while (low < high)
    {
        idx_t mid = low + (high - low) / 2;
        u128 cum = (u128)mid * (mid + 3) / 2;
        if (cum >= n)
            high = mid;
        else
            low = mid + 1;
    }
    return low;   /* r */
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* undefined, but return 0 */

    idx_t r = row_of_n(n);                /* 1‑based row number */
    size_t k = (size_t)(r - 1);           /* 0‑based polynomial index */
    u128 prev_cum = (u128)(r - 1) * (r + 2) / 2;  /* cumulative before this row */
    size_t pos = (size_t)(n - prev_cum - 1);      /* 0‑based position within row */

    /* compute rows up to k */
    if (rows_len <= k)
        ensure_rows_up_to(k);

    /* safety check – should never be out of bounds */
    if (k >= rows_len) return (seq_t)0;
    seq_t *row = rows[k];
    size_t row_len = k + 2;
    if (pos >= row_len) return (seq_t)0;

    return row[pos];
}
