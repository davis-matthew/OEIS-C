/*
 * A360832  Expansion of Sum_{k>=0} ( k * x / (1 - (k * x)^2) )^k.
 * https://oeis.org/A360832
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} (n-2*k)^n * binomial(n-k-1, k)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Helpers:   binomial, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/binomial.h"
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    seq_t result = 0;
    idx_t half = n / 2;
    for (idx_t k = 0; k <= half; ++k) {
        seq_t pow_term = pow_uint((uint64_t)(n - 2*k), n);
        seq_t bin = binomial(n - k - 1, k);
        result += pow_term * bin;
    }
    return result;
}
