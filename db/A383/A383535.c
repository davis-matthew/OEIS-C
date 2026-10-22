/*
 * A383535  Heinz number of the positive first differences of the 0-prepended
 *          prime indices of n.
 * https://oeis.org/A383535
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Heinz number of the positive first differences of the *
 *            0‑prepended prime indices of n
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   build_primes_upto, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/get_prime.h"
#include "../utils/build_primes_upto.h"

seq_t a(idx_t n)
{
    if (n == (idx_t)1) return (seq_t)1;

    /* Build list of primes up to n (needed both for factorisation
     * and for retrieving prime indices). */
    idx_t *prime_tmp = NULL;
    size_t prime_cnt = 0;
    build_primes_upto(n, &prime_tmp, &prime_cnt);          /* primes are idx_t (uint64_t) */
    uint64_t *primes       = (uint64_t *)prime_tmp;        /* cast to uint64_t for arithmetic */

    seq_t result = (seq_t)1;       /* the Heinz number */
    uint64_t rem   = (uint64_t)n;   /* remaining part to factorise */
    idx_t prev_idx = 0;             /* previous prime index in the sequence   */

    /* Factorise n using the prime list.  For each prime factor p with
     * multiplicity e we process its index e times, updating the
     * "previous index" and multiplying by prime(diff) where diff is the
     * positive difference between consecutive indices. */
    for (size_t i = 0; i < prime_cnt; ++i)
    {
        uint64_t p   = primes[i];
        idx_t     pidx = (idx_t)(i + 1);     /* 1‑based prime index */

        if (p > rem / p) break;             /* p*p > rem  (avoid overflow) */

        if (rem % p == 0)
        {
            unsigned exp = 0;
            while (rem % p == 0)
            {
                rem /= p;
                ++exp;
            }

            for (unsigned k = 0; k < exp; ++k)
            {
                idx_t diff = pidx - prev_idx;   /* positive difference? */
                if (diff > (idx_t)0)
                    result *= (seq_t)get_prime(diff);
                prev_idx = pidx;
            }
        }
    }

    /* If a prime factor remains (rem > 1), it is the last one.  Find its
     * index by scanning the prime list and process it. */
    if (rem > (uint64_t)1)
    {
        idx_t r = (idx_t)rem;
        idx_t ridx = 0;                      /* 1‑based index of 'rem' */

        for (size_t i = 0; i < prime_cnt; ++i)
            if (primes[i] == r)
            {
                ridx = (idx_t)(i + 1);
                break;
            }

        idx_t diff = ridx - prev_idx;
        if (diff > (idx_t)0)
            result *= (seq_t)get_prime(diff);
        /* prev_idx = ridx;  // not needed afterwards */
    }

    free(primes);
    return result;
}
