/*
 * A244754  a(n) = Sum_{k=0..n} C(n,k) * (1 + 2^k)^(n-k).
 * https://oeis.org/A244754
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} C(n,k) * (1 + 2^k)^(n-k)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 14, u128 n <= 20,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t result = 0;
    seq_t binom = 1;    /* C(n,0) */
    seq_t pow2k = 1;    /* 2^0 */

    for (idx_t k = 0; k <= n; ++k)
    {
        seq_t base = pow2k + 1;                  /* 1 + 2^k */
        seq_t pow  = big_pow(base, (seq_t)(n - k)); /* (1+2^k)^(n-k) */
        result += binom * pow;

        if (k == n) break;                      /* finished loop */

        /* update binomial: C(n,k+1) = C(n,k) * (n-k)/(k+1) */
        binom = binom * (seq_t)(n - k) / (seq_t)(k + 1);

        /* update 2^k for next iteration */
        pow2k *= 2;
    }
    return result;
}
