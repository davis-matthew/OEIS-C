/*
 * A321858  a(n) = Pi(12,5)(n) + Pi(12,7)(n) - Pi(12,1)(n) - Pi(12,11)(n)
 *          where Pi(a,b)(x) denotes the number of primes in the arithmetic
 *          progression a*k + b less than or equal to x.
 * https://oeis.org/A321858
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Pi(12,5)(n) + Pi(12,7)(n) - Pi(12,1)(n) - Pi(12,11)(n)
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of OEIS A321858: the difference in counts of primes
   congruent to 5 or 7 modulo 12 versus primes congruent to 1 or 11 modulo 12. */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* a(1) = 0 and all subsequent terms are computed by iterating over
       primes <= n and updating a running total. */
    if (n < 2)
        return 0;

    seq_t cur = 0;
    for (idx_t i = 2; i <= n; ++i)
    {
        if (is_prime(i))
        {
            idx_t r = i % 12;
            if (r == 5 || r == 7)
                cur++;
            else if (r == 1 || r == 11)
                cur--;
        }
    }
    return cur;
}
