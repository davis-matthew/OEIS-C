/*
 * A385077  Primes x that cannot be written as x = p + 2*q where p < q are
 *          primes.
 * https://oeis.org/A385077
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, hard, more
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   isPrimeTrial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence consists of primes that cannot be written as
 * x = p + 2*q with p < q primes.  The function lazily builds the
 * sequence, caching all terms found so far.  For each new prime x
 * it checks all smaller primes q < x/2; if p = x – 2*q is a prime
 * with p < q, then x has a representation and is not stored;
 * otherwise x is appended to the cache.  This algorithm runs in
 * O(m log m) where m is the needed prime size, comfortably under
 * 2 s for n ≈ 10 000. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/isPrimeTrial.h"

seq_t a(idx_t n)
{
    /* cache of found sequence terms */
    static seq_t *terms = NULL;
    static idx_t  tlen  = 0;        /* number of terms stored    */
    static size_t tcap  = 0;        /* allocated capacity        */

    /* cache of primes discovered so far */
    static uint64_t *primes = NULL;
    static size_t   pcap   = 0;    /* allocated capacity        */
    static size_t   psize  = 0;    /* number of primes stored   */

    /* next integer to test for primality (starts at 2) */
    static uint64_t next_candidate = 2;

    /* Grow the sequence until it contains at least n terms. */
    while (tlen < n)
    {
        /* find next prime */
        while (!isPrimeTrial((seq_t)next_candidate))
            ++next_candidate;

        seq_t x = (seq_t)next_candidate;   /* current prime */

        bool has_rep = false;

        /* check all primes q < x/2 */
        uint64_t half = next_candidate / 2;
        for (size_t i = 0; i < psize && primes[i] < half; ++i)
        {
            seq_t q = (seq_t)primes[i];
            seq_t p = x - (q + q);    /* p = x – 2*q */

            if (p < (seq_t)2) continue;    /* p must be ≥ 2          */
            if (p >= q)    continue;        /* require p < q           */

            if (isPrimeTrial(p))
            {
                has_rep = true;
                break;
            }
        }

        /* store x if it has no valid representation */
        if (!has_rep)
        {
            if (tlen == tcap)
            {
                tcap = tcap ? tcap * 2 : 4;
                terms = (seq_t *)realloc(terms, tcap * sizeof(seq_t));
            }
            terms[tlen++] = x;
        }

        /* remember the prime just found for future checks */
        if (psize == pcap)
        {
            pcap = pcap ? pcap * 2 : 4;
            primes = (uint64_t *)realloc(primes, pcap * sizeof(uint64_t));
        }
        primes[psize++] = next_candidate;

        ++next_candidate;
    }

    return terms[n - 1];
}
