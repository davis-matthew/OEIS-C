/*
 * A360481  E.g.f. satisfies A(x) = x * exp(x + 2 * A(x)).
 * https://oeis.org/A360481
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∑_{k=1..n} 2^(k-1) * k^(n-1) * binomial(n,k)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 17,
 *            big any n
 * Helpers:   binomial, seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function works for any supported seq_t and idx_t. */
#include "../utils/binomial.h"
#include "../utils/seq_pow.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)0;
    }

    seq_t sum = (seq_t)0;
    for (idx_t k = 1; k <= n; ++k) {
        seq_t binom    = binomial(n, k);
        seq_t two_pow  = seq_pow((seq_t)2, k - 1);
        seq_t k_pow    = seq_pow((seq_t)k, n - 1);
        seq_t term     = seq_mul(binom, two_pow);
        term           = seq_mul(term, k_pow);
        sum += term;
    }
    return sum;
}
