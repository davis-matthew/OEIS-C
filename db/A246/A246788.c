/*
 * A246788  Triangle read by rows: T(n,k) is the coefficient A_k in the
 *          transformation Sum_{k=0..n} (k+1)*x^k = Sum_{k=0..n} A_k*(x+2)^k.
 * https://oeis.org/A246788
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) = T(r,k) where r = floor((sqrt(8*n+1)-1)/2), k = n -
 *            r*(r+1)/2, and the triangle T(n,k) satisfies Sum_{k=0..n}
 *            (k+1)x^k = Sum_{k=0..n} T(n,k)(x+2)^k. T(n,n) = n+1 and for k <
 *            n T(n,k) = (k+1) – Σ_{j=k+1..n} T(n,j) * C(j,k) * 2^{j-k}
 * Range:     i8 n <= 14, i16 n <= 46, i32 n <= 54, i64 n <= 54, i128 n <= 54,
 *            big any n
 * Helpers:   binom, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binom.h"
#include "../utils/pow2.h"

seq_t a(idx_t n) {
    /* Find the triangle row r and column k for index n. */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n) {
        ++r;
    }
    idx_t k = n - r * (r + 1) / 2;

    /* Allocate array for the r‑th row. */
    seq_t *row = (seq_t *)malloc((r + 1) * sizeof(seq_t));
    if (row == NULL) return 0;  /* unlikely for small r */

    /* Back‑substitution: compute row entries from the top down. */
    for (idx_t i = r; ; --i) {
        seq_t sum = 0;
        for (idx_t j = i + 1; j <= r; ++j) {
            seq_t prod = row[j];
            prod *= binom(j, i);
            prod *= pow2(j - i);
            sum += prod;
        }
        row[i] = (seq_t)(i + 1) - sum;
        if (i == 0) break;
    }

    seq_t result = row[k];
    free(row);
    return result;
}
