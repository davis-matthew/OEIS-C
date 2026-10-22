/*
 * A360085  a(n) = Sum_{k=0..n} (-1)^k * binomial(4*k,n-k) * Catalan(k).
 * https://oeis.org/A360085
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=0..n} (-1)^k * binomial(4*k, n-k) * Catalan(k)
 * Range:     i8 n <= 5, i16 n <= 8, i32 n <= 16, i64 n <= 29, i128 n <= 29,
 *            big any n
 * Helpers:   binomial, catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/binomial.h"   /* prototype for binomial(idx_t, idx_t)   */
#include "../utils/catalan.h"     /* prototype for catalan(idx_t)          */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    bool neg = false;   /* sign of the current term; (-1)^k */

    for (idx_t k = 0; k <= n; ++k) {
        seq_t cat = catalan(k);                     /* Catalan(k)   */
        seq_t b   = binomial(4 * k, n - k);         /* binomial(4k, n-k) */
        seq_t term = cat * b;                       /* product      */

        if (neg) term = -term;                      /* apply (-1)^k */
        sum += term;                                /* accumulate   */

        neg = !neg;                                 /* toggle sign  */
    }

    return sum;
}
