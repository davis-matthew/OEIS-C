/*
 * A349862  a(n) is the maximum value of binomial(n-2*k,k) with 0 <= k <=
 *          floor(n/3).
 * https://oeis.org/A349862
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = max_{0 <= k <= floor(n/3)} binom(n-2*k, k)
 * Range:     u8 n <= 15, u16 n <= 28, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    idx_t maxk = n / 3;
    seq_t best = 0;
    for (idx_t k = 0; k <= maxk; ++k) {
        seq_t val = binom(n - 2 * k, k);
        if (val > best) best = val;
    }
    return best;
}
