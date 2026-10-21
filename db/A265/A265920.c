/*
 * A265920  Numerator of the probability that Alice wins the following game:
 *          Alice and Bob take turn (Alice starts first) to gain 1 or 2 chips
 *          randomly and independently with 1/2 chance, and the first player
 *          that collects at least n chips is the winner.
 * https://oeis.org/A265920
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (4^n + Sum_{k=1..n} 4^(n-k)*(binomial(k,
 *            n-k)+binomial(k-1, n-k))^2)/8
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* 4^n as seq_t */
    seq_t pow4n = ((seq_t)1) << ((uint64_t)(2 * n));

    seq_t sum = 0;
    /* pow will hold 4^(n-k) during the loop */
    seq_t pow = pow4n;

    for (idx_t k = 1; k <= n; ++k) {
        pow >>= 2;  /* pow = 4^(n-k)  */

        seq_t c1 = binomial(k, n - k);
        seq_t c2 = binomial(k - 1, n - k);
        seq_t s  = c1 + c2;

        seq_t sq  = s * s;
        seq_t term = pow * sq;

        sum += term;
    }

    seq_t total = pow4n + sum;
    return total / 8;
}
