/*
 * A397048  Triangle T(n, k) read by rows: T(n, k) = (Sum_{i=0..k} (-1)^i *
 *          binomial(k, i) * (n+i)^n) * (-1)^k / k! for 0 <= k <= n.
 * https://oeis.org/A397048
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 21, u32 n <= 54, u64 n <= 135, u128 n <= 377,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Triangle T(n,k) of OEIS A397048
 *  T(n,k) = T(n-1,k-1) + (n+2k)·T(n-1,k) + (n+k)(k+1)·T(n-1,k+1)
 *  with T(n,n)=1 and T(0,0)=1.
 *  a(n) is the flattened row‑major enumeration of the triangle:
 *      a(0)=T(0,0), a(1)=T(1,0), a(2)=T(1,1), a(3)=T(2,0), …
 *
 *  The function computes a(n) for any n≥0, caching all rows on the
 *  first call and reusing them thereafter.  No external dependencies
 *  beyond the standard <stdlib.h> header are used, and the code is
 *  valid both in C11 and C++17.
 */

#include <stdlib.h>

/* Helper: integer square root floor of an unsigned 64‑bit integer */
static idx_t isqrt_u64(idx_t x) {
    if (x < 2) return x;
    idx_t left = 1, right = x;
    while (left <= right) {
        idx_t mid = left + ((right - left) >> 1);
        if (mid <= x / mid)
            left = mid + 1;
        else
            right = mid - 1;
    }
    return right;
}

/* Main sequence function */
seq_t a(idx_t n) {
    /* Map the flattened index n to triangle coordinates (r,k) */
    idx_t D = 8 * n + 1;
    idx_t s = isqrt_u64(D);
    idx_t r = (s - 1) >> 1;              /* floor((sqrt(8n+1)-1)/2)   */
    idx_t k = n - r * (r + 1) / 2;        /* column index (0 ≤ k ≤ r) */

    /* Static cache of computed rows: rows[i] points to an array of i+1 seq_t values */
    static seq_t **rows = NULL;
    static idx_t max_row = 0;

    /* First call: allocate space for the zero‑th row */
    if (rows == NULL) {
        rows = (seq_t**)calloc(1, sizeof(seq_t*));
        if (!rows) return (seq_t)0;     /* allocation failure – return 0 */
        rows[0] = (seq_t*)malloc(sizeof(seq_t));
        if (!rows[0]) return (seq_t)0;
        rows[0][0] = (seq_t)1;          /* T(0,0) = 1 */
        max_row = 0;
    }

    /* Build rows up to r if necessary */
    if (r > max_row) {
        /* Extend the array of row pointers */
        seq_t **tmp = (seq_t**)realloc(rows, (r + 1) * sizeof(seq_t*));
        if (!tmp) return (seq_t)0;      /* allocation failure – return 0 */
        rows = tmp;

        for (idx_t i = max_row + 1; i <= r; ++i) {
            /* Allocate memory for the ith row (i+1 entries) */
            seq_t *row = (seq_t*)malloc((i + 1) * sizeof(seq_t));
            if (!row) return (seq_t)0;
            rows[i] = row;

            seq_t rn = (seq_t)i;      /* n = i in the recurrence */
            for (idx_t j = 0; j <= i; ++j) {
                if (j == i) {
                    /* Boundary condition T(i,i) = 1 */
                    rows[i][j] = (seq_t)1;
                } else {
                    /* Recurrence terms */
                    seq_t term1 = (j > 0) ? rows[i-1][j-1] : (seq_t)0;
                    seq_t term2 = (rn + (seq_t)2 * (seq_t)j) * rows[i-1][j];
                    seq_t term3 = (seq_t)0;
                    if (j + 1 <= i - 1) {
                        seq_t mult1 = rn + (seq_t)j;
                        seq_t mult2 = (seq_t)j + 1;
                        term3 = mult1 * mult2 * rows[i-1][j+1];
                    }
                    rows[i][j] = term1 + term2 + term3;
                }
            }
        }
        max_row = r;
    }

    return rows[r][k];
}
