/*
 * A337387  a(n) = Sum_{k=0..n} n^(n-k) * binomial(2*k,k) *
 *          binomial(2*n+1,2*k).
 * https://oeis.org/A337387
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} n^(n-k) * binomial(2*k,k) *
 *            binomial(2*n+1,2*k)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 17,
 *            big any n
 * Helpers:   binomial, pow_uint, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow_uint.h"
#include "../utils/binomial.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 0; k <= n; ++k)
    {
        seq_t power = pow_uint((uint64_t)n, n - k);
        seq_t b1    = binomial(2 * k, k);
        seq_t b2    = binomial(2 * n + 1, 2 * k);
        seq_t term  = seq_mul(seq_mul(power, b1), b2);
        sum += term;
    }
    return sum;
}
