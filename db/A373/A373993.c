/*
 * A373993  a(n) = 1 if the number of divisors of n divides n-1, otherwise 0.
 * https://oeis.org/A373993
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if the number of divisors of n divides n-1, otherwise 0
 * Range:     u8 n <= 100000, u16 n <= 100000, u32 n <= 100000,
 *            u64 n <= 100000, u128 n <= 100000, big any n
 * Validated: standard, pass, 141 terms through n = 9959 (2026-11-01)
 * Helpers:   num_divisors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/num_divisors.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* undefined for n = 0, return 0 for safety */
    idx_t d = (idx_t)num_divisors(n);   /* divisor count of n */
    return ((n - 1) % d == 0) ? 1 : 0;        /* 1 if d divides n-1, else 0 */
}
