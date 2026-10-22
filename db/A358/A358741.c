/*
 * A358741  Expansion of Sum_{k>=0} k! * ( k * x/(1 - x) )^k.
 * https://oeis.org/A358741
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 (n==0) a(n) = sum_{k=1..n} k! * k^k * binomial(n-1,
 *            k-1) (n>0)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 11, u128 n <= 15,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n) {
    if (n == 0)
        return (seq_t)1;

    seq_t sum   = 0;
    seq_t binom = 1;    // C(n-1, 0)
    seq_t fact  = 1;    // 0! initially

    for (idx_t k = 1; k <= n; ++k) {
        fact *= (seq_t)k;                               // k!
        seq_t powk = seq_pow((seq_t)k, k);              // k^k
        seq_t term = fact * powk;                       // k! * k^k
        term *= binom;                                  // * binom
        sum += term;                                    // accumulate

        if (k < n) {
            // Update binomial coefficient: C(n-1, k) = C(n-1, k-1) * (n-k) / k
            seq_t temp = binom * (seq_t)(n - k);
            binom = temp / (seq_t)k;
        }
    }

    return sum;
}
