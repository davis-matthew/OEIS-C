/*
 * A292889  Expansion of e.g.f. exp(x^3 * exp(x)).
 * https://oeis.org/A292889
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=0..floor(n/3)} k^(n-3*k)/(k! * (n-3*k)!)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 21, u128 n <= 35,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    seq_t result = 0;
    idx_t max_k = n / 3;
    for (idx_t k = 1; k <= max_k; ++k) {
        idx_t exp = n - 3 * k;

        /* Compute binomial coefficient bin[n, k] */
        seq_t bin = (seq_t)1;
        for (idx_t i = 1; i <= k; ++i) {
            bin = bin * (seq_t)(n - i + 1) / (seq_t)i;
        }

        /* Multiply by the product (n-3k+1) * ... * (n-k)   (2k terms) */
        idx_t start = n - 3 * k + 1;
        for (idx_t j = 0; j < 2 * k; ++j) {
            bin = bin * (seq_t)(start + j);
        }

        /* k^(n-3k) */
        seq_t pow_val = big_pow((seq_t)k, (seq_t)exp);

        result += bin * pow_val;
    }
    return result;
}
