/*
 * A360032  Expansion of Sum_{k>=0} (k * x * (1 + (k * x)^3))^k.
 * https://oeis.org/A360032
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/4)} (n-3*k)^n * binomial(n-3*k,k)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Special-case 0^0 as 1 */
    if (n == 0)
        return 1;

    idx_t maxk = n / 4;   /* floor(n/4) */
    seq_t sum = 0;

    for (idx_t k = 0; k <= maxk; ++k)
    {
        idx_t base_idx = n - 3 * k;        /* n-3k */
        seq_t base      = (seq_t)base_idx;
        seq_t power     = big_pow(base, (seq_t)n);      /* (n-3k)^n */
        seq_t binomial_val = binomial(base_idx, k);    /* binomial(n-3k,k) */
        sum += power * binomial_val;
    }
    return sum;
}
