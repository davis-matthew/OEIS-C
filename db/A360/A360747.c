/*
 * A360747  Expansion of Sum_{k>=0} (x * (1 + (k * x)^3))^k.
 * https://oeis.org/A360747
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/4)} (n-3k)^(3k) * binomial(n-3k, k)
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 16, u64 n <= 26, u128 n <= 44,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Requires the utilities big_pow and binomial. */
#include "../utils/big_pow.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 0; k <= n / 4; ++k)
    {
        idx_t rem = n - 3 * k;            /* n-3k   */
        seq_t base = (seq_t)rem;
        seq_t exp  = (seq_t)(3 * k);
        seq_t term = big_pow(base, exp);   /* (n-3k)^(3k) */
        seq_t bin  = binomial(rem, k);     /* binomial(rem, k) */
        term = term * bin;                 /* multiply */
        sum = sum + term;                  /* add to sum */
    }
    return sum;
}
