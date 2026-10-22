/*
 * A364304  a(n) = (7*n)!*(9*n/2)!*(5*n/2)!/((5*n)!*(7*n/2)!^2*n!^2).
 * https://oeis.org/A364304
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n} binomial(7*n, n-k)^2 * binomial(5*n + k - 1,
 *            k)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 8, u128 n <= 15,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    idx_t n7 = 7 * n;
    idx_t n5 = 5 * n;

    seq_t sum = 0;
    for (idx_t k = 0; k <= n; ++k) {
        seq_t b1 = binomial(n7, n - k);          /* binomial(7*n, n-k) */
        seq_t b2 = binomial(n5 + k - 1, k);      /* binomial(5*n + k - 1, k) */
        seq_t t = b1 * b1;
        t = t * b2;
        sum += t;
    }
    return sum;
}
