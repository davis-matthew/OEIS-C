/*
 * A253110  Primes p such that p, p^2, p^3 and p^4 lack the digit 0 in their
 *          decimal expansion.
 * https://oeis.org/A253110
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 15, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   append_candidate, hasZero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) is the n‑th prime p such that the decimal expansions of
   p, p², p³ and p⁴ contain no digit 0. The function builds the required
   numbers on demand, storing intermediate results for future calls. */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/append_candidate.h"   /* for append_candidate */
#include "../utils/hasZero.h"

seq_t a(idx_t n)
{
    /* static state to remember primes and qualifying primes */
    static seq_t *primeList = NULL;      /* list of all primes examined */
    static size_t primeCnt = 0, primeCap = 0;
    static seq_t *qualPrimes = NULL;     /* primes satisfying the 0‑digit-free condition */
    static size_t qualCnt = 0, qualCap = 0;
    static seq_t candidate = 2;          /* next number to test for primality */

    /* keep building until we have at least n qualifying primes */
    while (qualCnt < n)
    {
        bool isPrime = true;

        /* test primality of candidate by dividing by previously found primes */
        for (size_t i = 0; i < primeCnt; ++i)
        {
            seq_t p = primeList[i];
            /* avoid overflow: stop if p > sqrt(candidate) */
            if (candidate / p < p)
                break;
            if (candidate % p == 0)
            {
                isPrime = false;
                break;
            }
        }

        if (isPrime)
        {
            /* store the new prime */
            append_candidate(&primeList, &primeCnt, &primeCap, candidate);

            /* test the 0‑digit constraint on p, p², p³, p⁴ */
            if (!hasZero(candidate))
            {
                seq_t p2 = candidate * candidate;
                if (!hasZero(p2))
                {
                    seq_t p3 = p2 * candidate;
                    if (!hasZero(p3))
                    {
                        seq_t p4 = p3 * candidate;
                        if (!hasZero(p4))
                            append_candidate(&qualPrimes, &qualCnt, &qualCap, candidate);
                    }
                }
            }
        }

        /* advance to the next candidate: 2, then 3, 5, 7, … */
        candidate = (candidate == 2 ? 3 : candidate + 2);
    }

    return qualPrimes[n - 1];
}
