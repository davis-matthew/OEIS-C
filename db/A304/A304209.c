/*
 * A304209  Triangle read by rows: T(0,0)=1; T(n,k) =
 *          2*T(n-1,k)-2*T(n-1,k-1)+T(n-1,k-2), for k = 0, 1, ..., 2*n;
 *          T(n,k)=0 for n or k < 0.
 * https://oeis.org/A304209
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  tabf, easy, sign
 * Formula:   a(n) is the n‑th entry of the triangle T(0,0)=1;
 *            T(n,k)=2·T(n‑1,k)−2·T(n‑1,k‑1)+T(n‑1,k‑2), read by rows (row 0 →
 *            row 1 → …). This function reconstructs the required entry by
 *            generating the rows up to the needed row using only elementary
 *            integer arithmetic
 * Range:     i8 n <= 17, i16 n <= 63, i32 n <= 63, i64 n <= 63, i128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <math.h>

seq_t a(idx_t n)
{
    /* Special case n = 0 */
    if (n == 0) return (seq_t)1;

    /* Determine the row r such that r² ≤ n < (r+1)².
       r is the floor of sqrt(n).  Use double for speed,
       then correct possible rounding errors. */
    idx_t r = (idx_t)floor(sqrt((double)n));
    while ((r + 1) * (r + 1) <= n) ++r;
    while (r * r > n) --r;

    idx_t k = n - r * r;           /* column within the r‑th row */

    /* Allocate storage for two consecutive rows.
       The maximum row length needed is 2*r + 1. */
    size_t max_len = 2 * r + 1;
    seq_t *prev = (seq_t *)calloc(max_len, sizeof(seq_t));
    seq_t *curr = (seq_t *)calloc(max_len, sizeof(seq_t));
    if (!prev || !curr) {          /* allocation failure – return 0 */
        free(prev); free(curr);
        return (seq_t)0;
    }

    /* Initialize the 0‑th row */
    prev[0] = (seq_t)1;
    size_t prev_len = 1;

    /* Generate rows 1 … r one by one */
    for (idx_t row = 1; row <= r; ++row) {
        size_t curr_len = 2 * row + 1;
        for (size_t col = 0; col < curr_len; ++col) {
            seq_t a = (col < prev_len) ? prev[col] : (seq_t)0;
            seq_t b = (col >= 1 && col - 1 < prev_len) ? prev[col - 1] : (seq_t)0;
            seq_t c = (col >= 2 && col - 2 < prev_len) ? prev[col - 2] : (seq_t)0;
            curr[col] = ((seq_t)2 * a) - ((seq_t)2 * b) + c;
        }
        /* Swap the row buffers for the next iteration */
        seq_t *tmp     = prev;
        prev           = curr;
        curr           = tmp;
        prev_len       = curr_len;
    }

    /* Extract the requested entry */
    seq_t result = prev[k];

    free(prev);
    free(curr);
    return result;
}
