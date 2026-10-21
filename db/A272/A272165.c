/*
 * A272165  a(n) = Product_{k=0..n} (k*n-k)^k.
 * https://oeis.org/A272165
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Product_{k=0..n} (k*n-k)^k
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t result = 1;
    seq_t nm1 = (seq_t)(n - 1);
    for (idx_t k = 1; k <= n; ++k) {
        seq_t base = (seq_t)k * nm1;          /* k*(n-1) = k*n - k      */
        seq_t pow  = big_pow(base, (seq_t)k); /* (k*(n-1))^k             */
        result *= pow;
    }
    return result;
}
