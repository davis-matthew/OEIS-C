/*
 * A305866  a(n) is the number of primes that are equal to prime(n)+q+1, where
 *          prime(n) is the n-th prime number, q is a prime less than or equal
 *          to prime(n).
 * https://oeis.org/A305866
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of primes of the form prime(n)+q+1, where q runs
 *            over all primes ≤ prime(n)
 * Range:     u8 n <= 525, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "../utils/is_prime.h"
#include "../utils/build_primes_upto.h"

/* Cached list of primes */
static uint64_t *prime_list = NULL;
static size_t prime_cnt = 0;

/* Extend the cache until it contains at least n primes.
   The sieve is built up to enough size to test numbers
   prime(n)+q+1 (which is ≤ 2*prime(n)+1). */
static void ensure_primes(idx_t n_needed)
{
    if (prime_cnt >= (size_t)n_needed)
        return;

    while (prime_cnt < (size_t)n_needed) {
        /* Estimate an upper bound for the n-th prime using
           the classical inequality p_n < n (log n + log log n)
           for n ≥ 6.  */
        idx_t limit;
        if (n_needed < 6)
            limit = 15;
        else {
            double nd = (double)n_needed;
            limit = (idx_t)(nd * (log(nd) + log(log(nd)))) + 10;
        }

        /* Need to be able to test  prime(n)+q+1 ≤ 2*limit+1 */
        idx_t sieve_limit = limit * 2 + 10;

        uint64_t *new_primes = NULL;
        size_t new_cnt = 0;
        build_primes_upto(sieve_limit, &new_primes, &new_cnt);
        if (prime_list)
            free(prime_list);
        prime_list = new_primes;
        prime_cnt = new_cnt;

        /* If we still don't have enough primes, increase n_needed
           to the number we just computed and loop again. */
        if (prime_cnt < (size_t)n_needed)
            n_needed = prime_cnt;
    }
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                      /* Undefined for n=0, guard. */

    /* Make sure the prime list contains at least n primes. */
    ensure_primes(n);

    uint64_t p_n = prime_list[(size_t)n - 1];
    seq_t cnt = 0;

    /* Iterate over all primes q ≤ p_n. */
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t q = prime_list[i];
        if (q > p_n)
            break;
        uint64_t x = p_n + q + 1;      /* Candidate prime. */
        if (is_prime(x))
            ++cnt;
    }

    return cnt;
}
