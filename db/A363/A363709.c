/*
 * A363709  For n >= 0, a(n) is the least integer i >= 0 such that n + (n + 1)
 *          + ... + (n + i) is a prime number or a(n) = -1 if no such i
 *          exists.
 * https://oeis.org/A363709
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = least i ≥ 0 such that (i+1)*(2n+i)/2 is prime; a(0) = 2
 *            (since 0+1+2 = 3 is the first prime), for n ≥ 1 a(n) is 0 if n
 *            is prime, 1 if 2n+1 is prime, otherwise -1
 * Range:     i8 n <= 64, i16 n <= 80, i32 n <= 80, i64 n <= 80, i128 n <= 80,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n == 0u) {
        /* 0 + 1 + 2 = 3 is prime → minimal i = 2 */
        return (seq_t)2;
    }

    /* i = 0: sum = n */
    if (is_prime_big((seq_t)n))
        return (seq_t)0;

    /* i = 1: sum = n + (n+1) = 2*n + 1 */
    seq_t sum2 = (seq_t)2 * (seq_t)n + (seq_t)1;
    if (is_prime_big(sum2))
        return (seq_t)1;

    /* No i ≤ 1 produces a prime, so return -1 for n ≥ 1 */
    return (seq_t)-1;
}
