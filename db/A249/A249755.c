/*
 * A249755  Triangular array of coefficients of polynomials p(n,x) = (x +
 *          1)*p(n-1,x) + (n + 1)*x, p(0,x) = 1.
 * https://oeis.org/A249755
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = coefficient of x^k in p(r,x) where p(0,x) = 1, p(n,x) =
 *            (x+1)*p(n-1,x) + (n+1)*x, and n = r(r+1)/2 + k (0 ≤ k ≤ r)
 * Range:     u8 n <= 39, u16 n <= 159, u32 n <= 576, u64 n <= 2239,
 *            u128 n <= 5252, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* maximum row that the routine can handle (well above the 10 k range). */
#define MAX_R 20000

seq_t a(idx_t n)
{
    /* locate the triangular position (r,k) of index n */
    idx_t r = 0;
    idx_t start = 0;
    while (1) {
        idx_t row_len = r + 1;
        if (n < start + row_len) break;
        start += row_len;
        ++r;
    }
    idx_t k = n - start;

    /* arrays for successive rows */
    seq_t prev[MAX_R + 1];
    seq_t curr[MAX_R + 1];

    /* initialise row 0 */
    prev[0] = (seq_t)1;

    /* build rows 1 … r */
    for (idx_t curr_row = 1; curr_row <= r; ++curr_row) {
        curr[0] = prev[0];
        for (idx_t j = 1; j <= curr_row; ++j) {
            seq_t val = ((j < curr_row) ? prev[j] : (seq_t)0) + prev[j - 1];
            if (j == 1) {
                val += (seq_t)(curr_row + 1);
            }
            curr[j] = val;
        }
        /* copy current row to previous for next iteration */
        for (idx_t i = 0; i <= curr_row; ++i) {
            prev[i] = curr[i];
        }
    }

    /* return desired coefficient */
    return prev[k];
}
