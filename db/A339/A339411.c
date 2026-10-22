/*
 * A339411  Product of partial sums of odd squares.
 * https://oeis.org/A339411
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Product_{i=1..n} binomial(2*i + 1, 3)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t res = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i) {
        seq_t bin = binomial((idx_t)(2 * i + 1), (idx_t)3);
        res = res * bin;
    }
    return res;
}
