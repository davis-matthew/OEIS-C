/*
 * A373925  a(n) = Sum_{d|n} (-1)^phi(d).
 * https://oeis.org/A373925
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   divisor_count_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = d(n) - 3 - (-1)^n
 * where d(n) is the number of divisors of n.
 */

#include <stdint.h>
#include "../utils/divisor_count_u64.h"

seq_t a(idx_t n)
{
    uint64_t nn = n;
    seq_t dc = (seq_t)divisor_count_u64(nn);
    seq_t sign = (nn & 1ULL) ? (seq_t)-1 : (seq_t)1;
    return dc - 3 - sign;
}
