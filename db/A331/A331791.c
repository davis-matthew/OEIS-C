/*
 * A331791  Square array T(n,k), n >= 0, k >= 0, read by antidiagonals, where
 *          column k is the expansion of 2/(1 - 2*k*x + ((k-2)*x)^2 + (1 -
 *          k*x) * sqrt(1 - 2*k*x + ((k-2)*x)^2)).
 * https://oeis.org/A331791
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   not stated
 * Range:     i8 n <= 23, i16 n <= 59, i32 n <= 129, i64 n <= 342,
 *            i128 n <= 934, big any n
 * Helpers:   binom, seq_pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the n‑th element of the antidiagonal read of the square array
 * T(n,k) (n,k >= 0) defined by
 *    T(n,k) = Σ_{j=0}^{n} (k-1)^j * C(n+1,j) * C(n+1,j+1)
 * The OEIS entry shows that the sequence starts with
 * 1, 1, 0, 1, 2, -3, …
 * This implementation evaluates the sum directly; for each n it
 * determines the diagonal d and the position p on that diagonal,
 * then calls compute_T(row,col) with row=p and col=d-p.
 */

#include <stdint.h>            /* for uint64_t, etc. */
#include "../utils/binom.h"    /* declares binom(idx_t a, idx_t b) */
#include "../utils/seq_pow_int.h"

 

/* Compute T(n,k) via the explicit sum */
static seq_t compute_T(idx_t n, idx_t k)
{
    seq_t sum = 0;
    seq_t base = (seq_t)k - 1;   /* (k-1) */
    seq_t pow_term = 1;          /* (k-1)^0 initially */

    for (idx_t j = 0; j <= n; ++j) {
        if (j > 0)
            pow_term *= base;    /* incrementally compute (k-1)^j */

        seq_t bin1 = binom(n + 1, j);       /* C(n+1,j)   */
        seq_t bin2 = binom(n + 1, j + 1);   /* C(n+1,j+1) */

        sum += pow_term * bin1 * bin2;
    }
    return sum;
}

seq_t a(idx_t n)
{
    /* Find the diagonal d such that the cumulative count
     * up to the previous diagonal satisfies:
     *    d*(d+1)/2 <= n < (d+1)*(d+2)/2
     */
    idx_t d = 0;
    idx_t total = 0;
    while (total + d + 1 <= n) {
        total += d + 1;
        ++d;
    }

    /* Position m within diagonal d (0‑based) */
    idx_t m = n - total;          /* 0 <= m <= d */
    idx_t row = m;                /* r = m          */
    idx_t col = d - m;            /* c = d - m      */

    return compute_T(row, col);
}
