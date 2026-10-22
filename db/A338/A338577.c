/*
 * A338577  Primes p such that A013632(p)*A105161(p) > p.
 * https://oeis.org/A338577
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more, hard
 * Formula:   a(n) = the n-th prime p such that
 *            (next_prime(p)-p)*(next_next_prime(p)-p) > p. The function
 *            memoises all primes found so far and all qualifying primes (the
 *            “special” list) so that later calls do not recompute previous
 *            work
 * Range:     u8 n <= 13, u16 n <= 15, u32 n <= 15, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/isPrimeGeneric.h"   /* bool isPrimeGeneric(seq_t n); */

seq_t a(idx_t n)
{
    /* static state */
    static seq_t *primes   = NULL;
    static size_t primes_cap = 0, primes_cnt = 0;
    static seq_t *special   = NULL;
    static size_t special_cap = 0, special_cnt = 0;
    static seq_t  next_candidate = 2;   /* start checking from 2 */
    static idx_t  required = 0;         /* highest n requested so far */

    /* nothing defined below the offset */
    if (n == 0) return 0;

    /* update the maximum required index */
    if (n > required) required = n;

    /* generate primes and special primes until we have enough */
    while (special_cnt < (size_t)required)
    {
        /* find next prime starting from next_candidate */
        while (!isPrimeGeneric(next_candidate))
            ++next_candidate;

        /* store this prime */
        if (primes_cnt == primes_cap)
        {
            size_t new_cap = primes_cap ? primes_cap << 1 : 8;
            seq_t *tmp = (seq_t *)realloc(primes, new_cap * sizeof(seq_t));
            if (!tmp) return 0;          /* memory failure – return 0 */
            primes = tmp;
            primes_cap = new_cap;
        }
        primes[primes_cnt++] = next_candidate;

        /* if we have at least three consecutive primes, test the condition */
        if (primes_cnt >= 3)
        {
            seq_t p = primes[primes_cnt-3];
            seq_t q = primes[primes_cnt-2];
            seq_t r = primes[primes_cnt-1];

            /* compute (q-p)*(r-p) and compare with p */
            seq_t left = (q - p) * (r - p);
            if (left > p)
            {
                if (special_cnt == special_cap)
                {
                    size_t new_cap = special_cap ? special_cap << 1 : 8;
                    seq_t *tmp = (seq_t *)realloc(special, new_cap * sizeof(seq_t));
                    if (!tmp) return 0;
                    special = tmp;
                    special_cap = new_cap;
                }
                special[special_cnt++] = p;
            }
        }

        /* advance to the next integer candidate */
        ++next_candidate;
    }

    /* return the requested term; return 0 if n is larger than existing terms */
    return special[(size_t)(n-1)];
}
