/*
 * A360810  Expansion of Sum_{k>=0} ( x / (1 - k * x^2) )^k.
 * https://oeis.org/A360810
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..⌊n/2⌋} (n-2k)^k * binomial(n-k-1, k)
 * Range:     u8 n <= 8, u16 n <= 12, u32 n <= 21, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   binomial, seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/binomial.h"
#include "../utils/seq_pow.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    seq_t result = 0;
    /* iterate k from 0 to floor(n/2) */
    for (idx_t k = 0; k <= n >> 1; ++k) {
        /* base = n - 2*k  (convert to seq_t) */
        seq_t base = (seq_t)(n - 2 * k);

        /* power = base^k (using the provided seq_pow) */
        seq_t power = seq_pow(base, k);

        /* bin = binomial(n - k - 1, k) */
        seq_t bin = binomial(n - k - 1, k);

        /* term = power * bin (using seq_mul) */
        seq_t term = seq_mul(power, bin);

        result += term;
    }
    return result;
}
