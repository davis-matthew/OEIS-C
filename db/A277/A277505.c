/*
 * A277505  E.g.f.: -LambertW(-x)/(1-x).
 * https://oeis.org/A277505
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=0; for n≥0, a(n+1) = (n+1)*a(n) + (n+1)^n. This recurrence
 *            is equivalent to the sum definition: a(n) = Σ_{k=1..n}
 *            binomial(n,k) * k^(k-1) * (n-k)!
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 27,
 *            big any n
 * Helpers:   big_pow, mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"
#include "../utils/mul_small.h"

/* Compute the n‑th term of OEIS A277505. */
seq_t a(idx_t n)
{
    seq_t result = 0;                /* a(0) */
    for (idx_t i = 1; i <= n; ++i) {
        seq_t pow_term = big_pow((seq_t)i, (seq_t)(i - 1));
        result = mul_small(result, (unsigned int)i) + pow_term;
    }
    return result;
}
