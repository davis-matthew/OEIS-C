/*
 * A376510  a(n) is the number of pairs of primes p+q=2*(n+4) with 5 <= p <= n
 *          such that either p+6 or q+6 is also prime.
 * https://oeis.org/A376510
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   build_primes_upto, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm:
   a(n) = number of primes p, q with p + q = 2*(n+4),
          5 ≤ p ≤ n+4, and either p+6 or q+6 is also prime. */
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

#include "../utils/is_prime_small.h"
#include "../utils/build_primes_upto.h"

seq_t a(idx_t n)
{
    /* static cache of primes up to the largest number used so far */
    static uint64_t *primes = NULL;   /* sorted list of primes */
    static size_t    primes_cnt = 0;
    static idx_t     primes_limit = 0;    /* largest integer covered */

    idx_t S = 2 * (n + 4);          /* 2*(n+4) */
    idx_t limit = S + 6;            /* need up to p+6 or q+6 */

    /* rebuild prime list if necessary */
    if (limit > primes_limit) {
        uint64_t *newp = NULL;
        size_t newcnt = 0;
        build_primes_upto(limit, &newp, &newcnt);
        if (primes) free(primes);
        primes     = newp;
        primes_cnt = newcnt;
        primes_limit = limit;
    }

    seq_t count = 0;   /* result to return */

    idx_t upper_p = n + 4;   /* maximum allowed p */

    for (size_t i = 0; i < primes_cnt; ++i) {
        uint64_t p = primes[i];
        if (p < 5)                /* lower bound */
            continue;
        if (p > upper_p)          /* outside allowed range */
            break;

        uint64_t q = S - p;
        if (q < 2)                 /* must be prime ≥ 2 */
            continue;
        if (!is_prime_small(q))
            continue;

        /* Check additional prime condition */
        if (is_prime_small(p + 6) || is_prime_small(q + 6))
            ++count;
    }

    return count;
}
