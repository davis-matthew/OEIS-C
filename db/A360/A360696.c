/*
 * A360696  Expansion of Sum_{k>=0} (x * (1 + k^k * x))^k.
 * https://oeis.org/A360696
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} (n-k)^(k*(n-k)) * binomial(n-k,k)
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 7, u64 n <= 9, u128 n <= 13,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t maxk = n / 2;              /* floor(n/2) */
    for (idx_t k = 0; k <= maxk; ++k) {
        idx_t m = n - k;                 /* n-k                      */
        seq_t exp = (seq_t)(k * m);      /* k*(n-k)                 */
        seq_t base = (seq_t)m;           /* (n-k)                    */
        seq_t pow_val = big_pow(base, exp);
        seq_t bin = binomial(m, k);      /* binomial(n-k, k)         */
        sum += pow_val * bin;
    }
    return sum;
}
