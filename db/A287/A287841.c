/*
 * A287841  Number of iterations of number of distinct prime factors (A001221)
 *          needed to reach 1 starting at n (n is counted).
 * https://oeis.org/A287841
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(omega(n)) + 1 for n > 1; a(1) = 1
 * Range:     u8 n <= 9003, u16 n <= 9003, u32 n <= 9003, u64 n <= 9003,
 *            u128 n <= 9003, big any n
 * Helpers:   count_distinct_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  omega(n) is the number of distinct prime factors of n           */
#include <stdint.h>
#include "../utils/count_distinct_primes.h"

seq_t a(idx_t n)
{
    if (n == (idx_t)1) {
        return (seq_t)1;
    }
    /* compute the number of distinct prime factors of n */
    int omega = count_distinct_primes((uint64_t)n);
    /* recursive definition: a(n) = a(omega(n)) + 1 */
    return a((idx_t)omega) + (seq_t)1;
}
