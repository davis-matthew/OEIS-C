/*
 * A358738  Expansion of Sum_{k>=0} k! * ( x/(1 - k*x) )^k.
 * https://oeis.org/A358738
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1, n=0 ; for n>0, * a(n) = Sum_{k=1..n} k! * k^(n-k) *
 *            binomial(n-1, k-1) * (OEIS A358738)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 19,
 *            big any n
 * Helpers:   binomial, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_uint.h"
#include "../utils/binomial.h"

seq_t a(idx_t n) {
    if (n == 0) {
        return 1;
    }
    seq_t sum = 0;
    seq_t fact = 1;          /* factorial of k, updated iteratively */
    for (idx_t k = 1; k <= n; ++k) {
        fact *= (seq_t)k;                      /* k!  */
        seq_t pw   = pow_uint(k, n - k);       /* k^(n-k) */
        seq_t binom = binomial(n - 1, k - 1);  /* C(n-1,k-1) */
        seq_t term = fact * pw * binom;        /* k! * k^(n-k) * C(n-1,k-1) */
        sum += term;                           /* accumulate to a(n) */
    }
    return sum;
}
