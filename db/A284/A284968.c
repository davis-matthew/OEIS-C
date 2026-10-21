/*
 * A284968  Least hairpin family matchings with n edges that are both L&P and
 *          C&C whose leftmost edge is part of a hairpin.
 * https://oeis.org/A284968
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (Sum_{k=1..n} Catalan(k)) - n
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/catalan.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;           /* Offset: a(1) is the first term */

    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k)
        sum += catalan(k);        /* exact value from provided utility */

    return sum - (seq_t)n;        /* subtract n to obtain the sequence term */
}
