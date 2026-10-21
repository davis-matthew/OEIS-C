/*
 * A274472  Number of iterations of the Collatz recursion required to reach a
 *          prime number.
 * https://oeis.org/A274472
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 62, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: simulates the Collatz sequence starting at n until a prime is encountered.
   For prime n the value is 0.  The given helpers are used for primality testing. */
#include <stdbool.h>
#include "../utils/is_prime.h"      /* is_prime(idx_t)   */
#include "../utils/is_prime_big.h"  /* is_prime_big(seq_t) */

seq_t a(idx_t n)
{
    /* prime numbers give a(n)=0 */
    if (is_prime(n))
        return 0;

    seq_t cur = (seq_t)n;
    seq_t steps = 0;

    /* iterate until a prime number is reached */
    while (!is_prime_big(cur))
    {
        if ((cur & 1) == 0) /* even */
            cur = cur >> 1; /* cur /= 2 */
        else                /* odd */
            cur = 3 * cur + 1;
        steps++;
    }
    return steps;
}
