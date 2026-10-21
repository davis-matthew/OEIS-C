/*
 * A251021  Number of (n+1) X (1+1) 0..3 arrays with no 2 X 2 subblock having
 *          the sum of its diagonal elements greater than the maximum of its
 *          antidiagonal elements.
 * https://oeis.org/A251021
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of entries of M^n where M[i][j] == 1 iff for the two
 *            adjacent rows (x1,y1) -> (x2,y2) we have x1 + y2 <= max(y1 +
 *            x2). This counts (n+1)×2 arrays over 0..3 obeying the 2×2
 *            subblock rule. The sequence starts with a(1)=110
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 19, u64 n <= 45, u128 n <= 99,
 *            big any n
 * Helpers:   matrix_mult
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/matrix_mult.h"

seq_t a(idx_t n) {
    /* Transition matrix M (constant). */
    static seq_t M[16][16];
    static idx_t built = 0;
    if (!built) {
        idx_t i, j;
        for (i = 0; i < 16; ++i) {
            idx_t x1 = i >> 2;  /* first column of previous row */
            idx_t y1 = i & 3;   /* second column of previous row */
            for (j = 0; j < 16; ++j) {
                idx_t x2 = j >> 2;  /* first column of next row */
                idx_t y2 = j & 3;   /* second column of next row */
                idx_t diag = x1 + y2;
                idx_t anti = y1 > x2 ? y1 : x2; /* max of antidiagonal elements */
                M[i][j] = (seq_t)(diag <= anti);
            }
        }
        built = 1;
    }

    /* Compute M^n by exponentiation by squaring. */
    seq_t result[16][16], base[16][16], tmp[16][16];
    idx_t i, j;
    for (i = 0; i < 16; ++i) {
        for (j = 0; j < 16; ++j) {
            result[i][j] = (i == j) ? (seq_t)1 : (seq_t)0;
            base[i][j]   = M[i][j];
        }
    }

    idx_t exp = n;
    while (exp > 0) {
        if (exp & 1) {
            matrix_mult(result, base, tmp);
            for (i = 0; i < 16; ++i)
                for (j = 0; j < 16; ++j)
                    result[i][j] = tmp[i][j];
        }
        matrix_mult(base, base, tmp);
        for (i = 0; i < 16; ++i)
            for (j = 0; j < 16; ++j)
                base[i][j] = tmp[i][j];
        exp >>= 1;
    }

    /* Sum all entries of result to obtain a(n). */
    seq_t sum = 0;
    for (i = 0; i < 16; ++i)
        for (j = 0; j < 16; ++j)
            sum += result[i][j];
    return sum;
}
