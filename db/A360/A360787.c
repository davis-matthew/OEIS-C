/*
 * A360787  Expansion of Sum_{k>=0} x^k / (1 - (k*x)^2)^(k+1).
 * https://oeis.org/A360787
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} (n-2*k)^(2*k) * binomial(n-k,k)
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 14, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   big_pow, binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0) = 1, a(1) = 1, a(2) = 1, a(3) = 3, ... */

#include "../utils/big_pow.h"
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t maxk = n / 2;

    for (idx_t k = 0; k <= maxk; ++k)
    {
        seq_t base  = (seq_t)(n - 2 * k);
        seq_t exp   = (seq_t)(2 * k);
        seq_t pow_val;

        /* (base)^exp with 0^0 = 1 */
        if (k == 0)
            pow_val = 1;
        else
            pow_val = big_pow(base, exp);

        seq_t bin = binom(n - k, k);
        sum += pow_val * bin;
    }

    return sum;
}
