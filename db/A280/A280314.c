/*
 * A280314  Relationship of prime numbers to multiples of 6. The value of a(n)
 *          = 2 if n*6 has two neighboring primes, a(n) = 1 if only n*6+1 is
 *          prime, a(n) = -1 if only n*6-1 is prime, and a(n) = 0 if the
 *          neighbors of n*6 are both composite.
 * https://oeis.org/A280314
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 2 if both 6n-1 and 6n+1 are prime, * a(n) = 1 if only
 *            6n+1 is prime, * a(n) = -1 if only 6n-1 is prime, * a(n) = 0
 *            otherwise. * Offset: 1
 * Range:     i8 n <= 91, i16 n <= 91, i32 n <= 91, i64 n <= 91, i128 n <= 91,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* out of the defined offset; value irrelevant */
    idx_t n1 = 6 * n - 1;
    idx_t n2 = 6 * n + 1;
    bool p1 = is_prime(n1);
    bool p2 = is_prime(n2);
    if (p1 && p2) return 2;
    if (p1)        return -1;
    if (p2)        return 1;
    return 0;
}
