/*
 * A383844  a(n) is the number of occurrences of n in A024934.
 * https://oeis.org/A383844
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Computes a(n) for sequence A383844.
    a(n) is the number of integers k such that
        sum_{prime p <= k} (k % p) = n.
    For n > 21 the set of such k lies in [0..n];
    for smaller n we use a fixed search bound of 2000. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"   /* provides build_primes_upto */

seq_t a(idx_t n)
{
    /* Determine the search limit for k */
    idx_t limit = (n > 21) ? n : 2000;

    /* Get all primes <= limit */
    uint64_t *primes = NULL;
    size_t prime_cnt = 0;
    build_primes_upto(limit, &primes, &prime_cnt);
    if (prime_cnt == 0) /* no primes, only k=0,1,2 give sum 0 */
    {
        seq_t result = 0;
        for (idx_t k = 0; k <= limit; ++k)
        {
            if (k <= 2 && n == 0) ++result;
            else if (k > 2 && n == 0) ++result;   /* sums for k>2 are >0 */
        }
        return result;
    }

    seq_t count = 0;   /* result counter */

    /* Iterate over k = 0..limit and accumulate sums */
    for (idx_t k = 0; k <= limit; ++k)
    {
        /* Find number of primes <= k */
        size_t pi = 0;
        while (pi < prime_cnt && primes[pi] <= k) ++pi;

        /* Compute sum of remainders */
        seq_t sum = 0;
        for (size_t i = 0; i < pi; ++i)
            sum += (seq_t)(k % primes[i]);

        if (sum == (seq_t)n)
            ++count;
    }

    free(primes);
    return count;
}
