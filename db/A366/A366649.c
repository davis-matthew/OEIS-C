/*
 * A366649  Largest prime power (including 1) proper divisor of n, for n >= 2;
 *          a(1) = 1.
 * https://oeis.org/A366649
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = largest prime power proper divisor of n; a(1)=1
 * Range:     u8 n <= 511, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* For a given n ≥ 1, this function scans the prime factorisation of n
   and returns the maximum proper divisor that is a prime power.  For
   prime powers n = p^k the returned value is p^(k-1); for composites
   it is the largest of p^e (for each prime factor p) that is < n. */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the offset and trivial cases. */
    if (n <= 1)
        return (seq_t)1;

    uint64_t orig = n;           /* original value of n */
    uint64_t rem  = orig;        /* number remaining to be factored   */
    uint64_t best = 1;           /* best prime power found so far     */

    /* Factorisation by trial division. */
    for (uint64_t d = 2; d <= rem / d; ++d) {
        if (rem % d == 0) {                /* d is a prime factor  */
            uint64_t p   = d;              /* the prime factor     */
            uint64_t pow = 1;              /* p^e will accumulate */
            while (rem % p == 0) {         /* count its exponent   */
                rem /= p;
                pow *= p;
            }
            /* pow now equals p^e and divides orig.  If this power is
               equal to orig (i.e. n is itself a prime power),
               we must use the next smaller power, p^(e-1).            */
            if (pow == orig) {
                uint64_t cand = pow / p;   /* p^(e-1) */
                if (cand > best)
                    best = cand;
            } else {
                if (pow > best)
                    best = pow;
            }
        }
    }

    /* If rem > 1, then rem is a prime divisor larger than sqrt(orig). */
    if (rem > 1) {
        uint64_t p   = rem;
        uint64_t pow = p; /* exponent is 1 */
        if (pow == orig) {
            /* rem == orig, i.e. orig itself is prime, return 1. */
            if (1 > best)
                best = 1;
        } else {
            if (pow > best)
                best = pow;
        }
    }

    return (seq_t)best;
}
