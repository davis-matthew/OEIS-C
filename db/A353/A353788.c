/*
 * A353788  a(n) = 1 if the number of divisors of n is either 2 or 4,
 *          otherwise 0.
 * https://oeis.org/A353788
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if tau(n) == 2 or tau(n) == 4 else 0
 * Range:     u8 n <= 9735, u16 n <= 9735, u32 n <= 9735, u64 n <= 9735,
 *            u128 n <= 9735, big any n
 * Helpers:   divisor_count_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where tau(n) is the number of divisors of n. */
#include <stdbool.h>
#include <stdint.h>
#include "../utils/divisor_count_u64.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    uint64_t u = (uint64_t)n;
    uint64_t tau = divisor_count_u64(u);
    return (tau == 2ULL || tau == 4ULL) ? (seq_t)1 : (seq_t)0;
}
