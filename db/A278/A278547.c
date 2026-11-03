/*
 * A278547  Number of n X 1 0..3 arrays with rows and columns in lexicographic
 *          nondecreasing order but with exactly two mistakes.
 * https://oeis.org/A278547
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(n+11,n) - (n+1)*binomial(n+7,n) +
 *            binomial(n+3,n)*binomial(n+1,2)
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 31, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Validated: standard, pass, 44 terms through n = 190 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t t1 = binomial(n + 11, n);   /* binomial(n+11,n) = binomial(n+11,11) */
    seq_t t2 = binomial(n + 7,  n);   /* binomial(n+7,n)   = binomial(n+7,7)  */
    seq_t t3 = binomial(n + 3,  n);   /* binomial(n+3,n)   = binomial(n+3,3)  */
    seq_t twos = binomial(n + 1, 2);   /* binomial(n+1,2)                      */

    return t1 - (seq_t)(n + 1) * t2 + t3 * twos;
}
