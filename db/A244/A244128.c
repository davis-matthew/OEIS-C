/*
 * A244128  Triangle read by rows: coefficients T(n,k) of a binomial
 *          decomposition of 0^(n-1) as Sum(k=0..n)T(n,k)*binomial(n,k).
 * https://oeis.org/A244128
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabf
 * Formula:   a(n) = 0 if column k = 0; otherwise a(n) = (-1)^(k-1) * k^(r-1),
 *            where r, k are the row and column (0‑based) of the n‑th entry in
 *            the triangle read by rows (r = 1,2,3,...)
 * Range:     i8 n <= 18, i16 n <= 33, i32 n <= 74, i64 n <= 168,
 *            i128 n <= 432, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute the n-th term of sequence A244128.
   n is 1‑based. */
seq_t a(idx_t n)
{
    /* Find the row r such that n lies within it. */
    idx_t r = 1;           /* current row (1‑based)    */
    idx_t cumulative = 0;  /* number of elements before this row */

    /* Each row r has r+1 elements. */
    while (cumulative + (r + 1) < n) {
        cumulative += r + 1;
        ++r;
    }

    /* Column k within the row (0‑based). */
    idx_t k = n - cumulative - 1;

    if (k == 0) return 0; /* by definition */

    /* Compute k^(r-1) in seq_t. */
    seq_t power = 1;
    for (idx_t i = 0; i < r - 1; ++i) {
        power *= (seq_t)k;
    }

    /* Determine the sign: (-1)^(k-1). */
    int sign = ((k - 1) & 1) ? -1 : 1;

    return sign > 0 ? power : -power;
}
