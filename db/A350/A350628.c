/*
 * A350628  Number of ways to write 2*n as 3^i (i >= 1) plus a prime.
 * https://oeis.org/A350628
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = number of ways to write 2*n as 3^i (i >= 1) plus a prime.
 *
 * The implementation iterates over all powers of 3 up to 2*n-2 and
 * counts how many times the complementary number is prime.
 */

#include <stdint.h>
#include <stdbool.h>
#include <limits.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* 2*n may overflow idx_t when n is very large, but the sequence is
       defined for the usual range of n. In practice n is small enough
       that this works. */
    idx_t two_n = n * 2;

    /* For n >= 1, two_n >= 2, so two_n - 2 does not underflow. */
    seq_t count = 0;
    for (idx_t pow = 3; pow <= two_n - 2; )
    {
        idx_t p = two_n - pow;
        if (p >= 2 && is_prime(p))
            count++;

        /* Prevent overflow of pow when multiplying by 3. */
        if (pow > UINT64_MAX / 3)
            break;
        pow *= 3;
    }

    return count;
}
