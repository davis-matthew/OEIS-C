/*
 * A360819  Expansion of Sum_{k>=0} ( (k*x)^3 / (1 - k*x) )^k.
 * https://oeis.org/A360819
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0..⌊n/3⌋} k^n * binomial(n-2k-1, n-3k)
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 14, u64 n <= 21, u128 n <= 23,
 *            big any n
 * Helpers:   big_pow, binomial, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (k = 0 contributes 1 only when n = 0, handled separately) */

#include "../utils/big_pow.h"
#include "../utils/binomial.h"
#include "../utils/seq_mul.h"
#include <stddef.h>

static inline seq_t term_for(idx_t n, idx_t k)
{
    seq_t pow_val = big_pow((seq_t)k, (seq_t)n);
    seq_t bin = binomial(n - 2*k - 1, n - 3*k);
    return seq_mul(pow_val, bin);
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t sum = (seq_t)0;
    idx_t max_k = n / 3;
    for (idx_t k = 1; k <= max_k; ++k)
        sum += term_for(n, k);

    return sum;
}
