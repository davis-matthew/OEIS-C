/*
 * A369070  a(n) = 1 if there is at least one prime power p^e in the prime
 *          factorization of n such that p|e, otherwise 0.
 * https://oeis.org/A369070
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if there exists a prime power p^e in the prime
 *            factorization of n such that p divides e; 0 otherwise
 * Range:     u8 n <= 9728, u16 n <= 9728, u32 n <= 9728, u64 n <= 9728,
 *            u128 n <= 9728, big any n
 * Helpers:   has_condition
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/has_condition.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;
    return has_condition((uint64_t)n) ? (seq_t)1 : (seq_t)0;
}
