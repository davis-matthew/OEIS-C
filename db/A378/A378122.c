/*
 * A378122  a(n) = number of prime divisors of the sum of the first n primes.
 * https://oeis.org/A378122
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of distinct prime divisors of the sum of the first
 *            n primes
 * Range:     u8 n <= 15, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   count_distinct_primes, sieve_first_n_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/sieve_first_n_primes.h"
#include "../utils/count_distinct_primes.h"

seq_t a(idx_t n)
{
    if (n < 1u) return 0;
    /* Cache the first N primes for repeated use */
    static uint32_t *primes = NULL;
    static size_t max_primes = 0;

    if (n > max_primes) {
        free(primes);
        size_t cnt = 0;
        sieve_first_n_primes(n, &primes, &cnt);
        max_primes = cnt;
    }

    /* Compute the sum of the first n primes */
    seq_t sum = 0;
    for (idx_t i = 0; i < n; ++i) {
        sum += (seq_t)primes[i];
    }

    /* Count distinct prime divisors of the sum */
    /* sum is guaranteed to fit in uint64_t for n up to at least 10000,
       which covers the test range. */
    int distinct = count_distinct_primes((uint64_t)sum);
    return (seq_t)distinct;
}
