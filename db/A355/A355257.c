/*
 * A355257  Array read by ascending antidiagonals. A(n, k) = k! * [x^k] log((1
 *          - x) / (1 - 2*x)) / (1 - x)^n, for 0 <= k <= n.
 * https://oeis.org/A355257
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = A(i,j) where A(i,j) = j! * Sum_{m=1..j} binomial(i + j -
 *            m - 1, i - 1)*(2^m - 1)/m, and (i,j) is the pair obtained by
 *            reading the matrix A(n,k) by ascending antidiagonals
 * Range:     u8 n <= 19, u16 n <= 34, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   antidiagonal_row, binomial, factorial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <math.h>

/* Utility function headers */
#include "../utils/binomial.h"
#include "../utils/factorial.h"
#include "../utils/pow2.h"
#include "../utils/antidiagonal_row.h"

seq_t a(idx_t n) {
    /* Find diagonal */
    idx_t d = antidiagonal_row(n);
    /* Previous cumulative count */
    idx_t prev = (idx_t)(((unsigned __int128)d * (d + 1)) / 2);
    /* Offset within the diagonal */
    idx_t offset = n - prev;
    /* Coordinates (i,j) */
    idx_t i = d - offset;
    idx_t j = offset;

    /* If column j is zero, value is zero */
    if (j == 0) return 0;

    /* Special case i == 0 */
    if (i == 0) {
        seq_t pow2_j = pow2(j);           /* 2^j */
        seq_t fact_j_minus_1 = factorial(j - 1);
        return fact_j_minus_1 * (pow2_j - 1);
    }

    /* General case: compute sum over m = 1..j */
    seq_t fact_j = factorial(j);
    seq_t sum = 0;
    for (idx_t m = 1; m <= j; ++m) {
        idx_t binom_numer = i + j - m - 1;
        idx_t binom_denom = i - 1;          /* i >= 1, so denom >= 0 */
        seq_t binom_val = binomial(binom_numer, binom_denom);
        seq_t pow2_m   = pow2(m);
        seq_t term      = binom_val * (pow2_m - 1);
        term = term * (fact_j / m);
        sum += term;
    }
    return sum;
}
