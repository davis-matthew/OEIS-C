/*
 * A368625  Characteristic function of non-refactorable numbers (A159973).
 * https://oeis.org/A368625
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceil(n/d(n)) – floor(n/d(n)); equivalently, a(n) = 0 if n
 *            is divisible by its number of divisors d(n), otherwise a(n) = 1
 * Range:     u8 n <= 9457, u16 n <= 9457, u32 n <= 9457, u64 n <= 9457,
 *            u128 n <= 9457, big any n
 * Helpers:   divisor_count_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>                     /* uint64_t */
#include "../utils/divisor_count_u64.h"  /* uint64_t divisor_count_u64(uint64_t) */

seq_t a(idx_t n)
{
    /* Sequence is defined for n ≥ 1; for safety return 0 for n = 0. */
    if (n == 0)
        return (seq_t)0;

    /* d(n) is guaranteed to fit in 64 bits for the range of n we use. */
    uint64_t d = divisor_count_u64((uint64_t)n);

    /* n % d == 0  →  d divides n  →  integer value → a(n) = 0.
       Otherwise a(n) = 1. */
    return ((n % d) == 0) ? (seq_t)0 : (seq_t)1;
}
