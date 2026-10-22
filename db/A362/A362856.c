/*
 * A362856  Square array T(n,k), n >= 0, k >= 0, read by antidiagonals
 *          downwards, where T(n,k) = Sum_{j=0..n} (-k)^(n-j) * j^j *
 *          binomial(n,j).
 * https://oeis.org/A362856
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) is obtained by reading the square array T(n,k) by
 *            antidiagonals downwards, where T(n,k) = Σ_{j=0..n} (-k)^{n-j} *
 *            j^j * binomial(n,j)
 * Range:     i8 n <= 13, i16 n <= 26, i32 n <= 54, i64 n <= 54, i128 n <= 54,
 *            big any n
 * Helpers:   binomial, seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/seq_mul.h"
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* Determine the diagonal d and position j on that diagonal.
       The total number of elements up to (and including) diagonal d is
       (d+1)*(d+2)/2. We need the largest d such that d*(d+1)/2 <= n.
       The diagonal index can be found by solving d^2 + d - 2n >= 0. */
    /* Compute d by integer arithmetic to avoid floating point errors: */
    idx_t d = 0;
    /* Solve d*(d+1)/2 <= n < (d+1)*(d+2)/2 */
    /* We can use binary search or quadratic formula. For small n up to 10000
       a simple loop is more than fast enough. */
    while ((d + 1) * (d + 2) / 2 <= n) {
        d++;
    }

    idx_t start = d * (d + 1) / 2;          /* start index of this diagonal */
    idx_t j = n - start;                   /* position along diagonal, 0 <= j <= d */

    /* Coordinates in the square array: row = j, column = d - j */
    idx_t row = j;
    idx_t col = d - j;

    seq_t total = 0;
    for (idx_t jj = 0; jj <= row; ++jj) {
        /* Compute (-col)^(row-jj) */
        seq_t base_neg_col = -(seq_t)(col);
        seq_t pow_neg_col = seq_pow(base_neg_col, row - jj);

        /* Compute jj^jj */
        seq_t pow_jj = seq_pow((seq_t)jj, jj);

        /* Binomial coefficient */
        seq_t bin = binomial(row, jj);

        /* term = pow_neg_col * pow_jj * bin */
        seq_t term = seq_mul(pow_neg_col, seq_mul(pow_jj, bin));

        total += term;
    }
    return total;
}
