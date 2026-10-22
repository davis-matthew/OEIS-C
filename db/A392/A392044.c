/*
 * A392044  a(n) = Sum_{k=0..n} (k+1) * binomial(k,3*(n-k)).
 * https://oeis.org/A392044
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} (k+1) * binomial(k, 3*(n-k))
 * Range:     u8 n <= 7, u16 n <= 16, u32 n <= 33, u64 n <= 69, u128 n <= 142,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 0; k <= n; ++k)
    {
        idx_t t = 3 * (n - k);
        seq_t b = binomial(k, t);  /* 0 if t > k */
        sum += ((seq_t)(k + 1)) * b;
    }
    return sum;
}
