/*
 * A244142  Triangle read by rows: coefficients T(n,k) of a binomial
 *          decomposition of n as Sum(k=0..n)T(n,k)*binomial(n,k).
 * https://oeis.org/A244142
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) = T(r,k) where r(r+1)/2 ≤ n < (r+1)(r+2)/2, k = n -
 *            r(r+1)/2. T(r,0) = 0; T(1,1) = 1 (0⁰ convention); For r ≥ 2, k ≥
 *            2: T(r,k) = (-1)^k * k * (2*k-1)^(r-2). For all other (r,k)
 *            T(r,k)=0
 * Range:     i8 n <= 13, i16 n <= 25, i32 n <= 53, i64 n <= 132,
 *            i128 n <= 345, big any n
 * Helpers:   big_pow, get_row
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>
#include "../utils/big_pow.h"
#include "../utils/get_row.h"

seq_t a(idx_t n)
{
    /* Handle the trivial first element. */
    if (n == 0) return 0;

    /* Determine row r and column k. */
    idx_t r = get_row(n);
    idx_t k = n - r * (r + 1) / 2;

    /* Base cases. */
    if (k == 0)            return 0;         /* T(r,0)=0 */
    if (r == 1 && k == 1)  return 1;         /* T(1,1)=1 by convention */
    if (k <= 1)            return 0;         /* T(r,1)=0 for r ≥ 2 */

    /* r ≥ 2 and k ≥ 2: compute the formula. */
    seq_t base = (seq_t)(2 * k - 1);
    seq_t exp  = (seq_t)(r - 2);
    seq_t result = big_pow(base, exp);          /* (2k-1)^(r-2) */

    result *= (seq_t)k;                         /* multiply by k */
    if (k & 1)                                 /* apply (-1)^k */
        result = -result;

    return result;
}
