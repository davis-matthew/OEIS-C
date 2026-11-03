/*
 * A359473  a(n) = 1 if the number of divisors of n is a power of 2, otherwise
 *          0.
 * https://oeis.org/A359473
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = 1 if the number of divisors of n is a power of 2, else 0
 * Range:     u8 n <= 100000, u16 n <= 100000, u32 n <= 100000,
 *            u64 n <= 100000, u128 n <= 100000, big any n
 * Validated: standard, pass, 136 terms through n = 9415 (2026-11-01)
 * Helpers:   num_divisors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/num_divisors.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* n < 1 is undefined in OEIS but handled safely */
    uint64_t d = num_divisors((uint64_t)n);  /* number of divisors of n */
    /* check power of two: d>0 and d & (d-1) == 0 */
    return (d && (d & (d - 1)) == 0) ? (seq_t)1 : (seq_t)0;
}
