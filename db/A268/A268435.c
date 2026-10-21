/*
 * A268435  Triangle read by rows, T(n,k) = RF(n-k+1,n-k)*S2(n,k) where RF
 *          denotes the rising factorial and S2 the Stirling set numbers, for
 *          n>=0 and 0<=k<=n.
 * https://oeis.org/A268435
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   * T(0,0) = 1 * T(n,0) = 0 for n > 0 * T(n,n) = 1 for n > 0 *
 *            T(n,k) = k·(4·(n−k)−2)·T(n−1,k) + T(n−1,k−1) for 1 ≤ k ≤ n−1 * *
 *            a(n) is read from the triangle row‑wise: * let r be the smallest
 *            integer with r·(r+1)/2 > n * let c = n − r·(r+1)/2 * then a(n) =
 *            T(r,c). * * This implementation memoises...
 * Range:     u8 n <= 15, u16 n <= 28, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#ifdef __cplusplus
#include <new>  /* for placement new */
#endif

/* allocate an array of seq_t and construct it (C++ only) */
static seq_t *alloc_seq_array(size_t len)
{
    seq_t *p = (seq_t *)malloc(len * sizeof(seq_t));
    if (!p) {
        fprintf(stderr, "Out of memory\n");
        exit(EXIT_FAILURE);
    }
#ifdef __cplusplus
    for (size_t i = 0; i < len; ++i)
        new (&p[i]) seq_t();
#endif
    return p;
}

/* memoised triangle rows: rows[i] points to an array of length i+1 */
static seq_t **rows = NULL;
static idx_t rows_cap = 0;   /* capacity of rows array */
static idx_t rows_count = 0; /* number of rows already computed */

/* ensure rows up to max_row are computed */
static void compute_rows_up_to(idx_t max_row)
{
    if (max_row >= rows_cap) {
        seq_t **tmp = (seq_t **)
            realloc(rows, (size_t)(max_row + 1) * sizeof(seq_t *));
        if (!tmp) {
            fprintf(stderr, "Out of memory\n");
            exit(EXIT_FAILURE);
        }
        rows = tmp;
        rows_cap = max_row + 1;
    }

    for (idx_t i = rows_count; i <= max_row; ++i) {
        seq_t *row_arr = alloc_seq_array((size_t)i + 1);
        for (idx_t k = 0; k <= i; ++k) {
            seq_t val;
            if (i == 0 && k == 0) {
                val = 1;                     /* T(0,0) */
            } else if (k == 0) {
                val = 0;                     /* T(n,0) */
            } else if (k == i) {
                val = 1;                     /* T(n,n) */
            } else {
                /* T(n,k) = k·(4·(n−k)−2)·T(n−1,k) + T(n−1,k−1) */
                seq_t term1 = rows[i - 1][k];
                seq_t term2 = rows[i - 1][k - 1];
                seq_t factor = ((seq_t)k *
                               (((seq_t)4 * (seq_t)(i - k)) - (seq_t)2));
                val = factor * term1 + term2;
            }
            row_arr[k] = val;
        }
        rows[i] = row_arr;
    }
    rows_count = max_row + 1;
}

/* Return the nth term of sequence A268435. */
seq_t a(idx_t n)
{
    /* locate the row that contains index n */
    idx_t row = 0;
    while (((row + 1) * (row + 2) / 2) <= n) {
        ++row;
    }
    idx_t col = n - row * (row + 1) / 2;

    if (row >= rows_count) {
        compute_rows_up_to(row);
    }
    return rows[row][col];
}
