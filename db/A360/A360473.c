/*
 * A360473  E.g.f. satisfies A(x) = exp( x * exp(x) * A(x)^2 ).
 * https://oeis.org/A360473
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} k^(n-k) * (2*k+1)^(k-1) * binomial(n,k)
 *            (with a(0)=1; the summation effectively starts at k=1 for n>0)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 23,
 *            big any n
 * Helpers:   binomial, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/binomial.h"
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    seq_t sum = (seq_t)0;
    for (idx_t k = 1; k <= n; ++k) {
        seq_t term = binomial(n, k);
        term *= pow_uint((uint64_t)k, n - k);         /* k^(n-k)   */
        term *= pow_uint((uint64_t)(2 * k + 1), k - 1);/* (2k+1)^(k-1) */
        sum += term;
    }
    return sum;
}
