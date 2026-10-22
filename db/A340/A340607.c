/*
 * A340607  Number of factorizations of n into an odd number of factors > 1,
 *          the greatest of which is odd.
 * https://oeis.org/A340607
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of factorizations of n into an odd number of
 *            factors >1 where the greatest factor is odd
 * Range:     u8 n <= 9965, u16 n <= 9965, u32 n <= 9965, u64 n <= 9965,
 *            u128 n <= 9965, big any n
 * Helpers:   _factor_rec
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/_factor_rec.h"

seq_t a(idx_t n)
{
    /* Sequence starts with a(1)=0; for n < 2 no valid factorization exists. */
    if (n < 2) {
        return 0;
    }

    seq_t count = 0;
    _factor_rec((uint64_t)n, 2, 0, 0, &count);
    return count;
}
