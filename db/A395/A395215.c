/*
 * A395215  Square array A(n,k), n>=0, k>=0, read by antidiagonals downwards,
 *          where A(n,k) = (2*n)! * [x^(2*n)] 1/((1 - x)*(1 - 3*x))^(k/2).
 * https://oeis.org/A395215
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   A(0,k) = 1 A(n,k) = k*(k+2)*A(n-1,k+4) + 3*k*(k+1)*A(n-1,k+2)
 *            (n>0)
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 25, u64 n <= 52, u128 n <= 132,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is read by antidiagonals: a(n) = A(row,col) where
   (row,col) are the coordinates of n in the array. */

#include <stddef.h>   /* size_t */
#include <stdbool.h> /* bool */
#include <stdint.h>  /* uint64_t */

/* Pre‑allocate a small fixed array that is sufficient for all n
   up to about 10 000 (row ≤ 141, col ≤ 141, but the recurrence
   needs k up to ≈4*row; we over‑allocate to be safe). */
#define MAX_ROWS 200
#define MAX_K   (4 * MAX_ROWS + 10)

static seq_t A[MAX_ROWS][MAX_K];
static bool A_computed[MAX_ROWS][MAX_K] = { { false } };

/* Compute the table once on the first call. */
static void init_table(void)
{
    for (idx_t k = 0; k < MAX_K; ++k) {
        A[0][k] = 1;
        A_computed[0][k] = true;
    }

    for (idx_t n = 1; n < MAX_ROWS; ++n) {
        for (idx_t k = 0; k + 4 < MAX_K; ++k) {
            seq_t a2 = A[n - 1][k + 2];
            seq_t a4 = A[n - 1][k + 4];
            seq_t kk = (seq_t)k;

            seq_t term1 = kk * ((kk) + (seq_t)2) * a4;
            seq_t term2 = (seq_t)3 * kk * ((kk) + (seq_t)1) * a2;

            A[n][k] = term1 + term2;
            A_computed[n][k] = true;
        }
        /* values with k+4 ≥ MAX_K are never needed for the
           indices we support, so they remain zero. */
    }
}

/* Return (row,col) coordinates for the linear index n. */
static void antidiag_coords(idx_t n, idx_t *row, idx_t *col)
{
    idx_t t = 0;
    /* Find largest t with t*(t+1)/2 <= n. */
    while ((t + 1) * (t + 2) / 2 <= n)
        ++t;

    *row = n - t * (t + 1) / 2;
    *col = t - *row;
}

seq_t a(idx_t n)
{
    static bool initialized = false;
    if (!initialized) {
        init_table();
        initialized = true;
    }

    idx_t row, col;
    antidiag_coords(n, &row, &col);

    /* The input constraints guarantee that row < MAX_ROWS
       and col < MAX_K for all n up to the intended limit. */
    return A[row][col];
}
