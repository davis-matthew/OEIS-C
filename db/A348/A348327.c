/*
 * A348327  Characteristic function of numbers that have no middle divisors.
 * https://oeis.org/A348327
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n has no middle divisors, 0 otherwise. A middle
 *            divisor d satisfies √(n/2) ≤ d ≤ √(2n) (integers d dividing n).
 *            This function checks the interval for any divisor of n
 * Range:     u8 n <= 8546, u16 n <= 8546, u32 n <= 8546, u64 n <= 8546,
 *            u128 n <= 8546, big any n
 * Helpers:   ceil_sqrt_half_n, floor_sqrt_2n
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/ceil_sqrt_half_n.h"
#include "../utils/floor_sqrt_2n.h"

/* ------------------------- Main sequence function ---------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)          /* not used (offset is 1) but harmless */
        return (seq_t)0;

    idx_t high = floor_sqrt_2n(n);          /* maximal d with d² ≤ 2n */
    idx_t low  = ceil_sqrt_half_n(n);       /* minimal d with 2d² ≥ n */

    if (low > high)
        return (seq_t)1;                    /* empty interval → no middle divisor */

    for (idx_t d = low; d <= high; ++d) {
        if (n % d == 0)
            return (seq_t)0;                /* found a middle divisor */
    }
    return (seq_t)1;                        /* none found */
}
