/*
 * A385669  a(n) = Sum_{k=0..n} 2^k * 3^(n-k) * binomial(2*n+1,k) *
 *          binomial(2*n-k,n-k).
 * https://oeis.org/A385669
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} 5^k * 2^(n-k) * binomial(n+k,k)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 15, u128 n <= 17,
 *            big any n
 * Helpers:   binomial, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Utility includes */
#include "../utils/binomial.h"
#include "../utils/pow_uint.h"

seq_t a(idx_t n) {
    seq_t sum = 0;
    seq_t pow5 = 1;                     /* 5^k */
    seq_t pow2 = pow_uint(2, n);        /* 2^(n-k), start with k=0 */

    for (idx_t k = 0; k <= n; ++k) {
        seq_t c = binomial(n + k, k);
        /* sum += pow5 * pow2 * c */
        sum += pow5 * pow2 * c;
        pow5 *= (seq_t)5;               /* next 5^k */
        pow2 /= (seq_t)2;               /* next 2^(n-k-1) */
    }
    return sum;
}
