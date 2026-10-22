/*
 * A338505  Number of positive integers less than n with the same product of
 *          decimal digits as n.
 * https://oeis.org/A338505
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = |{0 < j < n : digit_product(j) = digit_product(n)}|
 * Range:     u8 n <= 507, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_product
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/digit_product.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;

    seq_t prod_n = digit_product((seq_t)n);
    seq_t count = 0;

    for (idx_t j = 1; j < n; ++j)
    {
        if (digit_product((seq_t)j) == prod_n)
            ++count;
    }

    return count;
}
