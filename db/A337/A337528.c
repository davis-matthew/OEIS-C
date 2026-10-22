/*
 * A337528  G.f.: Sum_{n>=0} (2^n + 1)^n * x^n / (1 + (2^n - 1)*x)^(n+1).
 * https://oeis.org/A337528
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(n,k) * (2^k+1)^(n-k) * (2^k-1)^k
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 5, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Helpers:   binomial, pow2, pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/binomial.h"
#include "../utils/pow_int.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 0; k <= n; ++k)
    {
        seq_t pow_two_k = pow2(k);        /* 2^k          */
        seq_t a_k  = pow_two_k + (seq_t)1;/* 2^k + 1      */
        seq_t b_k  = pow_two_k - (seq_t)1;/* 2^k - 1      */

        seq_t term = binomial(n, k);
        term *= pow_int(a_k, n - k);
        term *= pow_int(b_k, k);

        sum += term;
    }
    return sum;
}
