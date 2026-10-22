/*
 * A377774  a(n) = largest prime p < gpf(n) that does not divide n, or p =
 *          nextprime(gpf(n)) for n in A055932, where gpf = A006530.
 * https://oeis.org/A377774
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = largest prime p < gpf(n) that does not divide n, *
 *            otherwise a(n) = nextprime(gpf(n)). (gpf = A006530)
 * Range:     u8 n <= 262, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gpf, next_prime, prev_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gpf.h"
#include "../utils/next_prime.h"
#include "../utils/prev_prime.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* not defined, but safe */

    /* largest prime factor of n */
    uint64_t g = gpf((uint64_t)n);

    /* g <= 2: no smaller prime exists, use next prime after g */
    if (g <= 2)
        return (seq_t)next_prime(g);

    /* iterate over primes < g, descending, to find one not dividing n */
    uint64_t p = prev_prime(g);          /* largest prime < g */
    while (p >= 2)
    {
        if ((uint64_t)n % p != 0)
            return (seq_t)p;              /* found required prime */

        if (p <= 2) break;                /* avoid underflow */
        p = prev_prime(p);                /* next smaller prime */
    }

    /* all primes < g divide n; return next prime after g */
    return (seq_t)next_prime(g);
}
