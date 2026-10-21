/*
 * A250398  Composite numbers whose sum of divisors divides the sum of their
 *          unrelated numbers.
 * https://oeis.org/A250398
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 29, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   factor_u64, is_prime, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A250398 – Composite numbers whose sum of divisors divides
    the sum of their unrelated numbers.
    The code searches for composite n, computes the set of
    unrelated numbers (multiples of the proper divisors of n
    that are not divisors of n), sums them, and checks whether
    sigma(n) divides that sum.  The found composite numbers
    are cached and returned in order.                                                 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdint.h>        /* for __builtin_popcountll */
#include "../utils/is_prime.h"
#include "../utils/sigma_of.h"
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* Cache of already computed composite numbers */
    static seq_t *terms = NULL;
    static size_t tlen  = 0;
    static size_t tcap  = 0;
    static idx_t  last_candidate = 0;   /* largest candidate examined so far */

    if (n == 0)
        return 0;                /* undefined, but helper for safety */

    /* Return already computed value */
    if (n <= tlen)
        return terms[n - 1];

    /* Ensure the cache can hold at least n elements */
    if (tcap < n) {
        size_t newcap = (tcap == 0) ? 16 : tcap * 2;
        while (newcap < n)
            newcap <<= 1;
        if (terms == NULL)
            terms = (seq_t *)malloc(newcap * sizeof(seq_t));
        else
            terms = (seq_t *)realloc(terms, newcap * sizeof(seq_t));
        tcap = newcap;
    }

    /* Start searching from the smallest composite or from the last
       candidate examined in a previous call */
    for ( ; tlen < n ; ) {
        idx_t candidate;
        if (last_candidate < 4)
            candidate = 4;
        else
            candidate = last_candidate + 1;

        last_candidate = candidate;   /* remember the value for the next call */

        /* Skip primes and 1,2,3 */
        if (candidate <= 3) {
            continue;
        }
        if (is_prime(candidate)) {
            continue;
        }

        /* Compute sigma(n) */
        seq_t sigma = sigma_of(candidate);

        /* Factor the candidate into distinct primes */
        uint64_t primes[64];
        uint64_t exps[64];
        size_t   pfcnt = 0;
        factor_u64(candidate, primes, exps, &pfcnt);

        /* Inclusion–exclusion over the prime factors to get the sum
           of numbers < candidate that share at least one prime factor */
        seq_t union_sum = 0;
        idx_t subsets = ((idx_t)1) << pfcnt;      /* pfcnt ≤ 15 for 64‑bit */
        for (idx_t mask = 1; mask < subsets; ++mask) {
            idx_t prod = 1;
            for (size_t i = 0; i < pfcnt; ++i) {
                if (mask & ((idx_t)1 << i))
                    prod *= primes[i];
            }
            idx_t t = (candidate - 1) / prod;
            seq_t s = ((seq_t)prod * (seq_t)t * (seq_t)(t + 1)) / 2;
            if (__builtin_popcountll(mask) & 1)
                union_sum += s;
            else
                union_sum -= s;
        }

        /* Sum of unrelated numbers: subtract proper divisors (except 1) */
        seq_t sum_unrelated = union_sum - (sigma - (seq_t)candidate - (seq_t)1);

        /* Check divisibility condition */
        if (sigma != 0 && sum_unrelated % sigma == 0) {
            terms[tlen++] = (seq_t)candidate;
        }
    }

    return terms[n - 1];
}
