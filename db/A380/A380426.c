/*
 * A380426  E.g.f. A(x) satisfies A(x) = exp( 3 * x * A(x)^(1/3) * exp(x *
 *          A(x)^(1/3)) ).
 * https://oeis.org/A380426
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3 * Sum_{k=0..n} k^(n-k) * (n+3)^(k-1) * binomial(n,k)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 24,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: The n=0 term is defined to be 1. */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) return 1;

    seq_t sum = 0;
    seq_t binom = 1;                /* C(n,0) */
    seq_t pow_n3 = 1;               /* (n+3)^(k-1), start with k=1 => exponent 0 */
    const seq_t base_n3 = (seq_t)(n + 3);

    for (idx_t k = 1; k <= n; ++k) {
        /* Update binomial coefficient C(n,k) */
        binom = binom * ((seq_t)(n - k + 1));
        binom = binom / ((seq_t)k);   /* exact division */

        /* Compute k^(n-k) */
        const seq_t pow_k = big_pow((seq_t)k, (seq_t)(n - k));

        /* term = C(n,k) * k^(n-k) * (n+3)^(k-1) */
        seq_t term = binom * pow_k;
        term = term * pow_n3;

        sum = sum + term;

        /* Prepare pow_n3 for next k: multiply by (n+3) */
        pow_n3 = pow_n3 * base_n3;
    }

    return sum * 3;
}
