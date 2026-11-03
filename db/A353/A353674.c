/*
 * A353674  a(n) = 1 if n is an even number with an odd number of distinct
 *          prime factors, otherwise 0.
 * https://oeis.org/A353674
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n is even and has an odd number of distinct prime
 *            factors, otherwise 0
 * Range:     u8 n <= 65537, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 137 terms through n = 9969 (2026-11-01)
 * Helpers:   omega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/omega.h"

seq_t a(idx_t n)
{
    /* Only even n can be 1 */
    if ((n & 1) != 0)
        return (seq_t)0;

    /* Count distinct prime factors of n */
    int dpf = omega((uint64_t)n);

    /* Return 1 if the count is odd, else 0 */
    return (seq_t)(dpf & 1);
}
