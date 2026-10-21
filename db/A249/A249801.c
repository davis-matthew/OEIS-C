/*
 * A249801  Take smallest prime q such that n*(q+1)+1 is prime (A249800), that
 *          is, the smallest prime q so that n = (p-1)/(q+1) with p prime;
 *          sequence gives values of p; or -1 if A249800(n) = -1.
 * https://oeis.org/A249801
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(q+1)+1, where q is the smallest prime such that p =
 *            n*(q+1)+1 is prime. Returns p, or -1 if no such q exists within
 *            a practical bound
 * Range:     u8 n <= 30, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/is_prime_big.h"
#include "../utils/next_prime.h"

/* Practical upper bound for searching prime q. Adjust if more extreme cases are needed. */
#define MAX_Q 1000000ULL

seq_t a(idx_t n)
{
    if (n == 0)                         /* undefined for n = 0 */
        return (seq_t)-1;

    uint64_t q = 2;                     /* start at the smallest prime */

    while (q <= MAX_Q) {
        seq_t p = (seq_t)n * ((seq_t)q + 1) + 1;  /* compute p = n*(q+1)+1 */

        if (is_prime_big(p))           /* if p is prime, we found the answer */
            return p;

        /* move to the next prime */
        q = next_prime(q);              /* next_prime returns the next prime > q */
    }

    return (seq_t)-1;                   /* no suitable q found within limit */
}
