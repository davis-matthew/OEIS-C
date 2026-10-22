/*
 * A360731  Expansion of Sum_{k>=0} (k * x * (1 + k*x^3))^k.
 * https://oeis.org/A360731
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/4)} (n-3*k)^(n-2*k) * binomial(n-3*k,
 *            k)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    /* Iterate k from 0 to floor(n/4) */
    for (idx_t k = 0; k <= n / 4; ++k)
    {
        idx_t m   = n - 3 * k;   /* base: n-3k          */
        idx_t exp = n - 2 * k;   /* exponent: n-2k       */

        seq_t base   = (seq_t)m;
        seq_t binom  = binomial(m, k);
        seq_t power  = big_pow(base, (seq_t)exp);

        sum += power * binom;
    }
    return sum;
}
