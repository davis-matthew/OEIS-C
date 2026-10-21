/*
 * A292598  a(n) is the number of odd primes in the sequence [n, floor(n/2),
 *          floor(n/4), ..., 1].
 * https://oeis.org/A292598
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(floor(n/2)) + 1 if n is an odd prime, else
 *            a(floor(n/2)); a(1) = a(2) = 0. The sequence counts odd primes
 *            in the chain n, floor(n/2), floor(n/4), … , 1
 * Range:     u8 n <= 9968, u16 n <= 9968, u32 n <= 9968, u64 n <= 9968,
 *            u128 n <= 9968, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

/* Sequence value type seq_t and index type idx_t are supplied externally. */
seq_t a(idx_t n)
{
    seq_t count = 0;
    while (n > 2) {
        if (is_prime(n) && (n & 1ULL)) {
            count += (seq_t)1;
        }
        n >>= 1;                   /* equivalent to n = n / 2 */
    }
    return count;
}
