/*
 * A386919  a(n) = Sum_{k=0..n} binomial(4*n,k) * binomial(2*n-k,n-k).
 * https://oeis.org/A386919
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(4*n,k) * binomial(2*n-k,n-k)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 35,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Handle the only base case directly */
    if (n == 0)
        return (seq_t)1;

    seq_t sum = 0;
    seq_t b = 1;                     /* C(4*n, 0) */
    seq_t fk = binomial(2 * n, n);   /* C(2*n, n) for k = 0 */

    /* k = 0 term */
    sum += b * fk;

    for (idx_t k = 1; k <= n; ++k) {
        /* Update b to C(4*n, k) */
        b = (b * (4 * n - k + 1)) / k;

        /* Update fk to C(2*n-k, n-k) */
        fk = (fk * (n - k + 1)) / (2 * n - k + 1);

        sum += b * fk;
    }
    return sum;
}
