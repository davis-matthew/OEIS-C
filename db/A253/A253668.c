/*
 * A253668  Square array read by ascending antidiagonals, T(n, k) =
 *          k!*[x^k](log(x+1)*sum(j=0..n, C(n,j)*x^j)), n>=0, k>=0.
 * https://oeis.org/A253668
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   T(n,k) = k! * [x^k](log(1+x)*(1+x)^n), a(m) = T(n,m-n) with (n,
 *            k) read by ascending antidiagonals, ordering each diagonal with
 *            decreasing row index
 * Range:     i8 n <= 26, i16 n <= 43, i32 n <= 65, i64 n <= 65, i128 n <= 65,
 *            big any n
 * Helpers:   binomial, factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Include utilities for binomial coefficients and factorials. */
#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/factorial.h"

/* Return nth term of OEIS A253668 */
seq_t a(idx_t idx)
{
    /* Find diagonal d such that d*(d+1)/2 <= idx < (d+1)*(d+2)/2 */
    idx_t d = 0;
    while ((d + 1) * (d + 2) / 2 <= idx)
        ++d;

    /* Start index of this diagonal */
    idx_t start = d * (d + 1) / 2;

    /* Offset within the diagonal */
    idx_t offset = idx - start;

    /* k is the column index, offset within the diagonal */
    idx_t k = offset;

    /* n is the row index, computed as d - offset */
    idx_t n = d - offset;

    /* Compute k! */
    seq_t fact_k = factorial(k);

    /* Sum over m = 1 .. k of (-1)^{m+1} * C(n, k-m) * k! / m */
    seq_t res = 0;
    for (idx_t m = 1; m <= k; ++m) {
        idx_t j = k - m;          /* binomial index */
        if (j > n)                /* binomial is zero otherwise */
            continue;
        seq_t bin = binomial(n, j);
        seq_t term = bin * fact_k; /* multiply first to keep integers exact */
        term /= (seq_t)m;
        if (m & 1)                /* (-1)^{m+1} == +1 for odd m, -1 for even */
            res += term;
        else
            res -= term;
    }
    return res;
}
