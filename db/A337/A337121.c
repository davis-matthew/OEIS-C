/*
 * A337121  a(n) is the number of ways the n-th prime number prime(n) can be
 *          represented as sum of two smaller odd prime numbers p1, p2 with
 *          prime(n) > p1 > (p2 minus the maximum odd prime factor of
 *          (p1-p2)).
 * https://oeis.org/A337121
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of representations of prime(n) as p1 + p2 -
 *            MaxOddPrimeFactor(p1-p2)
 * Range:     u8 n <= 56, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   get_prime, is_prime_small, largest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Includes for required auxiliary functions */
#include <stdbool.h>
#include "../utils/get_prime.h"          /* uint64_t get_prime(idx_t k);  */
#include "../utils/largest_prime_factor.h" /* uint64_t largest_prime_factor(uint64_t n); */
#include "../utils/is_prime_small.h"    /* bool is_prime_small(seq_t num); */

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 4. For safety, return 0 for smaller indices. */
    if (n < 4)
        return 0;

    /* Obtain the nth prime. */
    uint64_t p = get_prime(n);          /* prime(n) as a 64‑bit unsigned integer */
    seq_t count = 0;                    /* result counter */

    /* Loop over all even differences d = p1 - p2, 2 <= d <= p-2. */
    for (uint64_t d = 2; d + 2 <= p; d += 2)
    {
        /* Determine the maximum odd prime factor of d. */
        uint64_t mp = largest_prime_factor(d);
        uint64_t f  = (mp == 2 ? 1 : mp);   /* if d is a power of two, f = 1 */

        /* Compute p1 and p2 from the equations:
           p1 = (p + f + d)/2
           p2 = (p + f - d)/2        */
        uint64_t sum = p + f + d;
        /* sum is always even: odd + odd + even. */
        uint64_t p1 = sum >> 1;
        uint64_t p2 = (p + f - d) >> 1;

        /* Conditions:
           - p1 and p2 must be odd primes less than p.
           - p2 must be at least 3 (exclude 2).
           - Ensure the additional inequality p1 > p2 - f.   */
        if (p1 >= p || p1 <= 2 || p2 < 3)
            continue;
        if (!is_prime_small((seq_t)p1) || !is_prime_small((seq_t)p2))
            continue;
        long long diff = (long long)p2 - (long long)f;
        if ((long long)p1 <= diff)
            continue;

        /* All conditions satisfied: count this representation. */
        ++count;
    }
    return count;
}
