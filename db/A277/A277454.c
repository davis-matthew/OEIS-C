/*
 * A277454  a(n) = 1 + Sum_{k=1..n} binomial(n,k) * 2^k * k^k.
 * https://oeis.org/A277454
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + Sum_{k=1..n} binomial(n,k) * 2^k * k^k
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 23,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS offset: 0 */

#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t sum = 0;
    seq_t binom = 1;      /* C(n,0) */
    seq_t two_pow = 1;    /* 2^0 */

    for (idx_t k = 1; k <= n; ++k) {
        /* Update binomial coefficient: C(n,k) = C(n,k-1)*(n-k+1)/k */
        binom *= (seq_t)(n - k + 1);
        binom /= (seq_t)k;

        /* Update 2^k */
        two_pow *= (seq_t)2;

        /* Compute k^k using power routine */
        seq_t k_pow = pow_uint((uint64_t)k, k);

        /* term = C(n,k) * 2^k * k^k */
        seq_t term = binom * two_pow * k_pow;

        sum += term;
    }

    return sum + (seq_t)1;
}
