/*
 * A348110  Number of positive integers <= n that have middle divisors.
 * https://oeis.org/A348110
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of k <= n that have a middle divisor (i.e., there
 *            exists a divisor d of k with sqrt(k/2) <= d <= sqrt(k))
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   has_middle_divisor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include "../utils/has_middle_divisor.h"

/* Main sequence function */
seq_t a(idx_t n)
{
    seq_t count = 0;
    for (idx_t i = 1; i <= n; ++i) {
        if (has_middle_divisor(i))
            ++count;
    }
    return count;
}
