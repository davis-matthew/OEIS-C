/*
 * A333175  If n = Product (p_j^k_j) then a(n) = Sum (a(n/p_j^k_j)), with a(1)
 *          = 1.
 * https://oeis.org/A333175
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = factorial(omega(n)), where omega(n) is the number of
 *            distinct prime factors of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   count_distinct_primes, factorial_uint32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/count_distinct_primes.h"  /* count_distinct_primes(uint64_t) */
#include "../utils/factorial_uint32.h"

seq_t a(idx_t n) {
    /* n is at least 1 (offset 1). */
    /* Count distinct prime factors of n. */
    int omega = count_distinct_primes((uint64_t)n);

    /* Return factorial of omega. */
    /* For omega = 0 (n=1), factorial(0) = 1. */
    return factorial_uint32((uint32_t)omega);
}
