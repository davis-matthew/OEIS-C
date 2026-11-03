/*
 * A306914  Square array A(n,k), n >= 0, k >= 1, read by antidiagonals, where
 *          column k is the expansion of g.f. 1/((1-x)^k+x^k).
 * https://oeis.org/A306914
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl, look
 * Formula:   a(n) = Sum_{j=0..⌊row/col⌋} (-1)^j * binomial(row+col-1,
 *            col*j+col-1) where (row, col) are derived from the antidiagonal
 *            index n
 * Range:     i8 n <= 58, i16 n <= 178, i32 n <= 610, i64 n <= 2307,
 *            i128 n <= 8708, big any n
 * Validated: standard, pass, 82 terms through n = 9769 (2026-11-01)
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/gcd.h"

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

static seq_t binomial_int(idx_t n, idx_t k) {
    if (k > n) return (seq_t)0;
    if (k > n - k) k = n - k;   /* use symmetry */
    seq_t res = (seq_t)1;
    for (idx_t i = 1; i <= k; ++i) {
        seq_t numer = (seq_t)(n - k + i);
        seq_t den   = (seq_t)i;
        seq_t g1 = gcd(numer, den);
        numer /= g1;
        den   /= g1;
        seq_t g2 = gcd(res, den);
        res /= g2;
        den   /= g2;           /* den should now be 1 */
        res = seq_mul(res, numer);
    }
    return res;
}

seq_t a(idx_t n) {
    /* find diagonal d such that d(d+1)/2 <= n < (d+1)(d+2)/2 */
    idx_t d = 0;
    while ((d + 1) * (d + 2) / 2 <= n)
        ++d;

    idx_t prev = d * (d + 1) / 2;          /* elements before this diagonal */
    idx_t row  = n - prev;                 /* row index (0‑based) */
    idx_t col  = d + 1 - row;              /* column index k (>=1) */

    seq_t sum = (seq_t)0;
    idx_t limit = row / col;               /* floor(row / col) */
    for (idx_t j = 0; j <= limit; ++j) {
        idx_t power = j * col + col - 1;   /* k*j + k - 1 */
        seq_t term  = binomial_int(row + col - 1, power);
        if (j & 1)
            sum -= term;
        else
            sum += term;
    }
    return sum;
}
