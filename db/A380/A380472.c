/*
 * A380472  a(n) = gcd_{primes P >= prime(n+1)} Product_{i=1..n} (P^2-i^2).
 * https://oeis.org/A380472
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n+2)! * (3/4-(-1)^n/4) (i.e. a(n) = (2n+2)! if n is
 *            odd, a(n) = (2n+2)!/2 if n is even)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factorial.h"

seq_t a(idx_t n)
{
    /* compute (2*n+2)! */
    idx_t m = 2*n + 2;
    seq_t fact = factorial(m);

    /* divide by 2 for even n */
    if ((n & 1) == 0)
        fact = fact / 2;

    return fact;
}
