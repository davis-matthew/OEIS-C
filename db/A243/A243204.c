/*
 * A243204  Expansion of
 *          2*x/((1-sqrt(1-2*(1-sqrt(1-4*x))))*sqrt(1-2*(1-sqrt(1-4*x))) *
 *          sqrt(1-4*x)).
 * https://oeis.org/A243204
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(2*k-1, k) * binomial(2*n-k-1, n-k)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 28, u128 n <= 54,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 0; k <= n; ++k) {
        seq_t bin1 = (k == 0) ? 1 : binom(2 * k - 1, k);
        seq_t bin2;
        if (n == 0 && k == 0) {
            bin2 = 1; /* binom(-1,0) is treated as 1 for n=0,k=0 */
        } else {
            bin2 = binom(2 * n - k - 1, n - k);
        }
        sum += bin1 * bin2;
    }
    return sum;
}
