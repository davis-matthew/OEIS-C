/*
 * A336041  Number of refactorable divisors of n.
 * https://oeis.org/A336041
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{d|n} [ tau(d) divides d ]
 * Range:     u8 n <= 9943, u16 n <= 9943, u32 n <= 9943, u64 n <= 9943,
 *            u128 n <= 9943, big any n
 * Helpers:   _refactorable_count_rec
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  where tau(d) is the number of divisors of d.     */

#include <stddef.h>
#include <stdint.h>
#include "../utils/_refactorable_count_rec.h"

/* Small primes up to 100, sufficient for n ≤ 10000. */
static const idx_t PRIMES[] = {2,3,5,7,11,13,17,19,23,29,
                               31,37,41,43,47,53,59,61,67,71,
                               73,79,83,89,97};
static const size_t NUM_PRIMES = sizeof(PRIMES)/sizeof(PRIMES[0]);

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined by OEIS, but safe */

    idx_t rem = n;
    idx_t primes[NUM_PRIMES];
    idx_t exps  [NUM_PRIMES];
    size_t pcnt = 0;

    /* Factorise n.  Only primes up to sqrt(n) are needed. */
    for (size_t i = 0; i < NUM_PRIMES && PRIMES[i] * PRIMES[i] <= rem; ++i) {
        if (rem % PRIMES[i] == 0) {
            idx_t e = 0;
            while (rem % PRIMES[i] == 0) {
                rem /= PRIMES[i];
                e++;
            }
            primes[pcnt] = PRIMES[i];
            exps[pcnt]   = e;
            pcnt++;
        }
    }
    /* If a prime factor larger than sqrt(n) remains. */
    if (rem > 1) {
        primes[pcnt] = rem;
        exps[pcnt]   = 1;
        pcnt++;
    }

    return _refactorable_count_rec(0, (idx_t)1, (idx_t)1,
                                   primes, exps, pcnt);
}
