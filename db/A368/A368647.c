/*
 * A368647  The number of distinct primes of the form 3*k+2 dividing n minus
 *          the number of distinct primes of the form 3*k+1 dividing n.
 * https://oeis.org/A368647
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (# of distinct primes p|n with p ≡ 2 (mod 3)) – (# of
 *            distinct primes p|n with p ≡ 1 (mod 3)) (p = 3 contributes 0).
 *            Only distinct prime factors are counted
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   _smallest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/_smallest_prime_factor.h"

seq_t a(idx_t n)
{
    if (n <= 1)                   /* a(1) = 0 */
        return 0;

    uint64_t x = (uint64_t)n;     /* working copy for factorisation */
    seq_t    result = 0;          /* running total */

    uint64_t last_p = 0;          /* last prime factor seen (for distinctness) */

    while (x > 1) {
        uint64_t p = _smallest_prime_factor(x);  /* smallest prime factor of current x */

        /* If this is a new prime factor, update the result based on p mod 3. */
        if (p != last_p) {
            if (p % 3 == 2)        /* p ≡ 2 (mod 3) */
                result += 1;
            else if (p % 3 == 1)   /* p ≡ 1 (mod 3) */
                result -= 1;
            /* p = 3 gives no change */
            last_p = p;
        }

        /* Remove all occurrences of p from x */
        while (x % p == 0)
            x /= p;
    }

    return result;
}
