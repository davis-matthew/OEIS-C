/*
 * A363866  a(n) = A143007(3*n,2*n).
 * https://oeis.org/A363866
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..2*n} binomial(3*n + k, 2*k) * binomial(2*k,
 *            k)^2 * binomial(2*n + k, 2*k)
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 3, u64 n <= 5, u128 n <= 10,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t maxk = 2 * n;
    for (idx_t k = 0; k <= maxk; ++k) {
        seq_t t = binomial(3 * n + k, 2 * k);
        seq_t b = binomial(2 * k, k);
        t = t * b * b;                 /* t = binomial(3*n+k,2*k) * binomial(2*k,k)^2 */
        seq_t c = binomial(2 * n + k, 2 * k);
        sum = sum + t * c;
    }
    return sum;
}
