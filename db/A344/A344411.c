/*
 * A344411  Sum of the prime numbers in the interval (2n, 3n].
 * https://oeis.org/A344411
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=2*n+1..3*n} k * (pi(k)-pi(k-1))
 * Range:     u8 n <= 20, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    seq_t sum = 0;
    idx_t low  = 2 * n;
    idx_t high = 3 * n;

    /* sum all primes in the interval (2n, 3n] */
    for (idx_t k = low + 1; k <= high; ++k)
        if (is_prime(k))
            sum += (seq_t)k;

    return sum;
}
