/*
 * A384195  a(n) = tau(n+1) - tau(n-1), where tau(n) = A000005(n), the number
 *          of divisors of n.
 * https://oeis.org/A384195
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = tau(n + 1) - tau(n - 1)
 * Range:     i8 n <= 85, i16 n <= 85, i32 n <= 85, i64 n <= 85, i128 n <= 85,
 *            big any n
 * Helpers:   divisor_count_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where tau(n) is the number of divisors of n */
#include <stdint.h>
#include "../utils/divisor_count_u64.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 2.  Calls with n < 2 are not
       expected; if they occur the behaviour is undefined. */
    uint64_t d_plus  = divisor_count_u64((uint64_t)(n + 1));
    uint64_t d_minus = divisor_count_u64((uint64_t)(n - 1));
    return (seq_t)d_plus - (seq_t)d_minus;
}
