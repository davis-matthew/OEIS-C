/*
 * A353675  a(n) = 1 if n is an odd number with an even number of distinct
 *          prime factors, otherwise 0.
 * https://oeis.org/A353675
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n is odd and has an even number of distinct prime
 *            factors, otherwise 0
 * Range:     u8 n <= 65537, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 137 terms through n = 9906 (2026-11-01)
 * Helpers:   omega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/omega.h"

seq_t a(idx_t n)
{
    /* respect the OEIS offset: a(1) is defined, a(0) is not part of the sequence */
    if (n < 1) return 0;

    /* even numbers cannot satisfy the condition */
    if ((n & 1) == 0) return 0;

    /* count the distinct prime factors of n */
    int distinct = omega((uint64_t)n);

    /* return 1 if the count is even, else 0 */
    return (distinct % 2 == 0) ? 1 : 0;
}
