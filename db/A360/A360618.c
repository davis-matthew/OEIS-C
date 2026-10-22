/*
 * A360618  Expansion of Sum_{k>=0} (k * x * (1 + k*x))^k.
 * https://oeis.org/A360618
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} (n-k)^n * binomial(n-k,k)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 25,
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
    idx_t maxk = n / 2;          /* floor(n/2) */

    for (idx_t k = 0; k <= maxk; ++k) {
        idx_t base = n - k;            /* (n-k) */
        seq_t power = big_pow((seq_t)base, (seq_t)n);        /* (n-k)^n */
        seq_t comb  = binomial(base, k);                    /* C(n-k,k) */
        sum += power * comb;   /* add term to the sum */
    }

    return sum;
}
