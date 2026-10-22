/*
 * A373993  a(n) = 1 if the number of divisors of n divides n-1, otherwise 0.
 * https://oeis.org/A373993
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if the number of divisors of n divides n-1, otherwise 0
 * Range:     u8 n <= 9959, u16 n <= 9959, u32 n <= 9959, u64 n <= 9959,
 *            u128 n <= 9959, big any n
 * Helpers:   divisor_count_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/divisor_count_u64.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* undefined for n = 0, return 0 for safety */
    idx_t d = (idx_t)divisor_count_u64(n);   /* divisor count of n */
    return ((n - 1) % d == 0) ? 1 : 0;        /* 1 if d divides n-1, else 0 */
}
