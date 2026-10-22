/*
 * A362464  Array of numbers read by upward antidiagonals: leading row lists
 *          sigma(i), i >= 1 (cf. A000203); the following rows give absolute
 *          values of differences of previous row.
 * https://oeis.org/A362464
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look, tabl
 * Formula:   a(n) = read upward antidiagonals of the infinite table where row
 *            0 is σ(k) for k ≥ 1, and each subsequent row r (r ≥ 1) contains
 *            |row r‑1[i] – row r‑1[i+1]|. The sequence starts: 1, 2, 3, 1, 1,
 *            4, …
 * Range:     u8 n <= 5884, u16 n <= 9482, u32 n <= 9482, u64 n <= 9482,
 *            u128 n <= 9482, big any n
 * Helpers:   absdiff_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/absdiff_seq_t.h"
#define MAX_SIZE 200

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined, harmless */

    /* Find the diagonal d such that the total number of elements
       up to that diagonal is at least n.  Total terms up to diagonal d
       equals (d+1)(d+2)/2. */
    idx_t d = 0;
    idx_t total = 1;              /* (0+1)(0+2)/2 = 1 */
    while (total < n) {
        ++d;
        total += d + 1;           /* add next diagonal length (d+1) */
    }
    const idx_t max_cols = d + 1; /* number of columns in row 0 */

    /* Allocate fixed-size arrays.  MAX_SIZE is large enough for all
       n up to 10000 (d never exceeds 141). */
    seq_t sigma[MAX_SIZE + 1] = {0};
    seq_t rows[MAX_SIZE][MAX_SIZE] = {{0}};

    /* Compute σ(k) for k = 1 .. max_cols */
    for (idx_t k = 1; k <= max_cols; ++k) {
        seq_t sum = 0;
        for (idx_t j = 1; j <= k / j; ++j) {
            if (k % j == 0) {
                sum += (seq_t)j;
                idx_t kdiv = k / j;
                if (kdiv != j) sum += (seq_t)kdiv;
            }
        }
        sigma[k] = sum;
    }

    /* Build the triangular array up to row d */
    for (idx_t r = 0; r <= d; ++r) {
        idx_t width = max_cols - r;
        if (r == 0) {
            for (idx_t c = 0; c < width; ++c)
                rows[0][c] = sigma[c + 1];
        } else {
            for (idx_t c = 0; c < width; ++c)
                rows[r][c] = absdiff_seq_t(rows[r - 1][c], rows[r - 1][c + 1]);
        }
    }

    /* Read the array by upward antidiagonals until the nth element is
       found.  For diagonal d the elements are (r, d‑r) with r from d
       down to 0. */
    idx_t count = 0;
    for (idx_t diag = 0; diag <= d; ++diag) {
        for (idx_t r = diag; ; --r) {
            idx_t col = diag - r;
            seq_t val = rows[r][col];
            ++count;
            if (count == n) return val;
            if (r == 0) break;
        }
    }

    /* Unreachable for valid n */
    return 0;
}
