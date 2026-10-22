/*
 * A355494  Expansion of Sum_{k>=0} (k * x/(1 - x))^k.
 * https://oeis.org/A355494
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} k^k * binomial(n-1,k-1) (a(0)=1)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   pow_exp
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_exp.h"

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)1;
    seq_t result = (seq_t)0;
    seq_t binom   = (seq_t)1;   /* binomial(n-1,0) */
    for (idx_t k = (idx_t)1; k <= n; ++k) {
        seq_t pk   = pow_exp((seq_t)k, k);
        result += pk * binom;
        if (k < n) {
            binom = binom * (seq_t)(n - k) / (seq_t)k; /* next binomial coeff */
        }
    }
    return result;
}
