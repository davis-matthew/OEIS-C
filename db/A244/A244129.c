/*
 * A244129  Triangle read by rows: terms of a binomial decomposition of
 *          0^(n-1) as Sum(k=0..n)T(n,k).
 * https://oeis.org/A244129
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) = (-1)^(k-1) * k^(n-1) * binomial(n,k) * where r is the row
 *            number such that r(r+3)/2 >= n, * k = n - (r-1)(r+2)/2 - 1, and
 *            k is the column index. * This is the (n,k) entry of the binomial
 *            decomposition triangle * defined in OEIS A244129
 * Range:     i8 n <= 16, i16 n <= 31, i32 n <= 61, i64 n <= 146,
 *            i128 n <= 393, big any n
 * Helpers:   binomial, pow_seq_large
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/pow_seq_large.h"

seq_t a(idx_t n)
{
    /* Find row r such that cumulative count up to r >= n: S(r)=r(r+3)/2 */
    idx_t r = 1;
    while ((r * (r + 3)) / 2 < n) {
        ++r;
    }

    /* Compute column index k */
    idx_t prev_sum = ((r - 1) * (r + 2)) / 2;   /* cumulative up to row r-1 */
    idx_t k = n - prev_sum - 1;                 /* 0‑based column index */

    if (k == 0) {
        return 0;  /* by convention */
    }

    /* Compute binomial coefficient */
    seq_t bin = binomial(r, k);

    /* Compute k^(r-1) */
    seq_t pow = pow_seq_large((seq_t)k, (seq_t)(r - 1));

    /* Combine to get T(r,k) */
    seq_t term = bin * pow;

    /* Adjust sign: (-1)^(k-1) */
    if (((k - 1) & 1) != 0) {
        term = -term;
    }

    return term;
}
