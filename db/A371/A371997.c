/*
 * A371997  Triangle read by rows: T(m, n, k) = 1 if k = 0 and T(m, n, k - 1)
 *          if k = n; otherwise (-1)^m*(k - n - 1)^m * T(m, n, k - 1) + T(m, n
 *          - 1, k) where m = 4.
 * https://oeis.org/A371997
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 24, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Triangle recurrence for m = 4:
 * T(m, n, k) = 1                     for k = 0
 *              T(m, n, k-1)          for k = n
 *              (-1)^m * (k - n - 1)^m * T(m, n, k-1) + T(m, n-1, k)
 *  for m = 4, (-1)^4 = 1, so (k - n - 1)^4 = (n + 1 - k)^4.
 *
 * The sequence a(n) is the triangle read by rows:
 *      a(0) = T(4,0,0)
 *      a(1) = T(4,1,0), a(2) = T(4,1,1)
 *      a(3) = T(4,2,0), a(4) = T(4,2,1), a(5) = T(4,2,2)
 *      ...
 */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* static caches for rows already computed
   rows[r] points to an array of size r+1 containing T(4,r,k) for k=0..r */
static seq_t **rows = NULL;
static idx_t   rows_count = 0;   /* number of rows allocated (rows_count-1 is last row index) */

/* compute (x^4) for small x */
static seq_t pow4(seq_t x) {
    return x * x * x * x;
}

/* main function requested by the build */
seq_t a(idx_t n) {
    /* determine row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n) r++;
    idx_t k = n - r * (r + 1) / 2;

    /* initialize row cache on first call */
    if (rows == NULL) {
        rows = (seq_t**)malloc(sizeof(seq_t*));
        rows[0] = (seq_t*)malloc(sizeof(seq_t));
        rows[0][0] = 1;
        rows_count = 1;
    }

    /* grow cache up to required row r */
    while (rows_count <= r) {
        idx_t prevR = rows_count - 1;
        seq_t *prev = rows[prevR];
        seq_t *curr = (seq_t*)malloc((rows_count + 1) * sizeof(seq_t));
        curr[0] = 1;                /* k = 0 case */
        for (idx_t kk = 1; kk < rows_count; ++kk) {
            seq_t diff = (seq_t)(rows_count + 1 - kk);   /* (n+1-k) for current row */
            seq_t factor = pow4(diff);
            curr[kk] = factor * curr[kk - 1] + prev[kk];
        }
        curr[rows_count] = curr[rows_count - 1];   /* k = n case */
        rows[rows_count] = curr;
        rows_count++;
    }

    return rows[r][k];
}
