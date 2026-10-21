/*
 * A244143  Triangle read by rows: terms T(n,k) of a binomial decomposition of
 *          n as Sum(k=0..n)T(n,k).
 * https://oeis.org/A244143
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) is the n‑th entry of the triangle T(n,k) defined by T(n,k)
 *            = 0 for k = 0, T(1,1) = 1 T(n,1) = 0 for n ≥ 2, k = 1 T(n,k) =
 *            (-1)^k * k * (2*k-1)^(n-2) * binomial(n,k) for k ≥ 2
 * Range:     i8 n <= 12, i16 n <= 23, i32 n <= 49, i64 n <= 113,
 *            i128 n <= 312, big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Locate the row r such that
         r*(r+1)/2 ≤ n < (r+1)*(r+2)/2                        (row start index)
       and find the column k = n - r*(r+1)/2 within that row.
       A simple linear scan is sufficient because r ≈ √(2n) (≤ ~140 for n≤10000). */

    idx_t r = 0;
    idx_t start = 0;                     /* start index of current row */
    while (start + (r + 1) <= n) {
        start += (r + 1);
        ++r;
    }
    idx_t k = n - start;                 /* column within the row */

    /* Special cases */
    if (r == 0)                     /* row 0 : [0] */
        return (seq_t)0;
    if (k == 0)
        return (seq_t)0;                /* T(n,0) = 0 for all n */
    if (r == 1 && k == 1)             /* T(1,1) = 1 */
        return (seq_t)1;
    if (k == 1)                       /* T(n,1) = 0 for n ≥ 2 */
        return (seq_t)0;

    /* k ≥ 2 : use the defining formula */
    seq_t bin = binomial(r, k);                                 /* binomial(r,k)   */
    seq_t pow_term = big_pow((seq_t)(2 * k - 1), (seq_t)(r - 2)); /* (2k-1)^(r-2)    */
    seq_t prod = bin * pow_term * (seq_t)k;                     /* k * bin * pow    */

    if (k & 1)                          /* apply the (-1)^k factor */
        prod = -prod;

    return prod;
}
