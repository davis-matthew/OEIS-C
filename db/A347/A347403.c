/*
 * A347403  Step at which n is removed by the sieve of Eratosthenes or 0 if n
 *          is prime.
 * https://oeis.org/A347403
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) equals the step at which n is removed by the sieve of
 *            Eratosthenes (0 if n is prime). For composite n, a(n) = π(p) +
 *            1, where p is the smallest prime factor of n. For n = 1, a(1) =
 *            1
 * Range:     u8 n <= 9718, u16 n <= 9718, u32 n <= 9718, u64 n <= 9718,
 *            u128 n <= 9718, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"

static uint64_t *primes = NULL;     /* list of primes up to PRIME_LIMIT */
static size_t prime_cnt = 0;        /* number of primes in the list */
static const size_t PRIME_LIMIT = 2000000; /* enough for n ≤ 1e12 */
static bool initialized = false;

/* initialise the prime list once */
static void init_primes(void)
{
    build_primes_upto(PRIME_LIMIT, &primes, &prime_cnt);
    initialized = true;
}

/* return the smallest prime factor of nn, or 0 if nn is prime */
static uint64_t smallest_prime_factor(uint64_t nn)
{
    if (nn < 2) return 0;
    for (size_t i = 0; i < prime_cnt; ++i)
    {
        uint64_t p = primes[i];
        if (p * p > nn) break;
        if (nn % p == 0) return p;
    }
    /* no divisor found – nn is prime */
    return 0;
}

/* count how many stored primes are ≤ x */
static size_t count_primes_leq(uint64_t x)
{
    size_t i = 0;
    while (i < prime_cnt && primes[i] <= x) ++i;
    return i;
}

/* the required OEIS function */
seq_t a(idx_t n)
{
    if (!initialized) init_primes();

    if (n == 1) return (seq_t)1;

    uint64_t nn = (uint64_t)n;
    uint64_t spf = smallest_prime_factor(nn);
    if (spf == 0) return (seq_t)0;          /* n is prime */

    size_t cnt = count_primes_leq(spf);     /* π(spf) */
    return (seq_t)(cnt + 1);                /* step number */
}
