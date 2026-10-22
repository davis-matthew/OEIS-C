/*
 * A337257  Number of even divisors of n!.
 * https://oeis.org/A337257
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of even divisors of n! = d(n!) - d_odd(n!)
 * Range:     u8 n <= 10, u16 n <= 21, u32 n <= 49, u64 n <= 118,
 *            u128 n <= 282, big any n
 * Helpers:   build_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0) = 0, a(1) = 0, a(2) = 1, a(3) = 2, a(4) = 6, …         */

#include <stddef.h>                 /* for size_t */
#include "../utils/build_primes.h"  /* build_primes(limit,&out,&cnt) */

/* static cache of primes up to the largest n requested so far */
static idx_t *primes   = NULL;  /* array of primes ≤ primes_max */
static size_t primes_cnt = 0;   /* number of primes in the array */
static idx_t  primes_max  = 0;  /* maximum n for which primes are generated */

seq_t a(idx_t n)
{
    /* n = 0 or 1 → 1! = 1, has no even divisors */
    if (n < 2) {
        return 0;
    }

    /* Ensure we have all primes up to n.  Rebuild the cache only when a larger
       bound is requested.  The build_primes routine allocates the array and
       sets the count; we do not free it on subsequent calls. */
    if (!primes || n > primes_max) {
        build_primes(n, &primes, &primes_cnt);
        primes_max = n;
    }

    seq_t total = 1;   /* d(n!)  : total number of divisors of n! */
    seq_t odd   = 1;   /* d_odd(n!): number of odd divisors of n! */

    for (size_t i = 0; i < primes_cnt; ++i) {
        idx_t p = primes[i];
        if (p > n) {
            break;
        }

        /* exponent of prime p in n!  (Legendre's formula) */
        idx_t t = n;
        idx_t exp = 0;
        while (t) {
            t /= p;
            exp += t;
        }

        seq_t factor = (seq_t)(exp + 1);  /* (exp+1) contributes to divisor count */
        total *= factor;
        if (p != 2) {                    /* ignore prime 2 for odd divisor count */
            odd *= factor;
        }
    }

    return total - odd;   /* exclude odd divisors */
}
