/*
 * A354747  Start with 2*n-1; repeatedly triple and add 2 until reaching a
 *          prime. a(n) = number of steps until reaching a prime > 2*n-1, or 0
 *          if no prime is ever reached.
 * https://oeis.org/A354747
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the smallest integer m ≥ 1 such that 2*n*3^m – 1 is
 *            prime. Computation: start with x = 2*n – 1, then repeatedly
 *            compute x = 3*x + 2 until a prime is found or a preset step
 *            limit is reached. Return the found step count (as seq_t) or 0 if
 *            the limit is exceeded
 * Range:     u8 n <= 15, u16 n <= 20, u32 n <= 20, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"

#define MAX_STEPS 200000ULL   /* safety limit for very large n with no prime */

seq_t a(idx_t n)
{
    /* initial value: 2*n – 1 */
    seq_t x = (seq_t)2 * n - (seq_t)1;

    for (uint64_t m = 1; m <= MAX_STEPS; ++m) {
        /* apply the map x -> 3*x + 2 */
        x = x * (seq_t)3 + (seq_t)2;

        /* check primality */
        if (is_prime_big(x))
            return (seq_t)m;
    }

    /* reached the step limit – no prime found so far */
    return (seq_t)0;
}
