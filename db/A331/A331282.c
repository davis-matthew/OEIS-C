/*
 * A331282  If A061395(n) < A001222(n), then a(n) = 1, otherwise a(n) = 0.
 * https://oeis.org/A331282
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8522, u16 n <= 8522, u32 n <= 8522, u64 n <= 8522,
 *            u128 n <= 8522, big any n
 * Helpers:   build_primes_upto, int_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Condition: a(n) = 1 if the sum of distinct prime factors of n is
   less than the number of divisors of n (A001222(n)), otherwise 0. */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"
#include "../utils/int_sqrt.h"

/* Global, cached list of primes up to the maximum sqrt(n) seen. */
static uint64_t *primes = NULL;
static size_t   primes_cnt = 0;
static idx_t    primes_limit = 0;

/* Initialise or extend the prime table to at least “limit”. */
static void ensure_primes(idx_t limit)
{
    if (primes_limit >= limit) return;
    free(primes);
    build_primes_upto(limit, &primes, &primes_cnt);
    primes_limit = limit;
}

/* Main sequence entry point. */
seq_t a(idx_t n)
{
    if (n == 1)                     /* special case: a(1) = 0 */
        return (seq_t)0;

    idx_t limit = int_sqrt(n);
    ensure_primes(limit);

    idx_t  temp = n;
    seq_t  tau  = (seq_t)1;     /* divisor count starting value */
    seq_t  sum  = (seq_t)0;     /* sum of distinct prime factors */

    for (size_t i = 0; i < primes_cnt; ++i) {
        uint64_t p = primes[i];
        if ((idx_t)p * (idx_t)p > temp) break;

        if (temp % p == 0) {
            sum += (seq_t)p;               /* distinct prime factor added once */
            int exp = 0;
            do {
                temp /= p;
                ++exp;
            } while (temp % p == 0);

            tau *= (seq_t)(exp + 1);       /* update divisor count */
        }
    }

    /* Remaining factor > 1 is prime. */
    if (temp > 1) {
        sum += (seq_t)temp;
        tau *= (seq_t)2;
    }

    return (sum < tau) ? (seq_t)1 : (seq_t)0;
}
