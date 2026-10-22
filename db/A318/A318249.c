/*
 * A318249  a(n) = (n - 1)! * d(n), where d(n) = number of divisors of n
 *          (A000005).
 * https://oeis.org/A318249
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n-1)! * d(n)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 21, u128 n <= 34,
 *            big any n
 * Helpers:   divisor_count_u64, factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factorial.h"
#include "../utils/divisor_count_u64.h"

seq_t a(idx_t n)
{
    seq_t f = factorial(n-1);          /* (n-1)!  */
    uint64_t d = divisor_count_u64((uint64_t)n);  /* d(n) */
    return f * (seq_t)d;               /* (n-1)! * d(n) */
}
