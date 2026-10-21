/*
 * A249800  a(n) is the smallest prime q such that n(q+1)+1 is prime, that is,
 *          the smallest prime q such that n = (p-1)/(q+1) with p prime; or
 *          a(n) = -1 if no such q exists.
 * https://oeis.org/A249800
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_primes_upto, is_prime_big, is_prime_small, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A249800: a(n) is the smallest prime q such that n*(q+1)+1 is prime,
   or -1 if no such prime exists. */

#include <stdbool.h>
#include "../utils/is_prime_small.h"
#include "../utils/is_prime_big.h"
#include "../utils/build_primes_upto.h"
#include "../utils/next_prime.h"

static uint64_t *prime_cache = NULL;
static size_t prime_cnt = 0;
static bool primes_initialized = false;
static const idx_t PRIMES_LIMIT = 2000000;

static void
init_primes(void)
{
    if (primes_initialized)
        return;
    build_primes_upto(PRIMES_LIMIT, &prime_cache, &prime_cnt);
    primes_initialized = true;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)-1;       /* below OEIS offset – undefined value */

    init_primes();

    /* Check the primes we already have. */
    for (size_t i = 0; i < prime_cnt; ++i)
    {
        uint64_t q = prime_cache[i];
        seq_t p = (seq_t)n * ((seq_t)q + 1) + 1;
        if (is_prime_big(p))
            return (seq_t)q;
    }

    /* If not found, extend the search beyond the initial sieve. */
    uint64_t q_start = prime_cache[prime_cnt - 1] + 1;
    uint64_t q = next_prime(q_start);        /* first prime > q_start */
    const uint64_t MAX_Q = 10000000ULL;     /* practical upper bound */

    while (q <= MAX_Q)
    {
        seq_t p = (seq_t)n * ((seq_t)q + 1) + 1;
        if (is_prime_big(p))
            return (seq_t)q;
        q = next_prime(q + 1);
    }

    /* No suitable q found within the bound. */
    return (seq_t)-1;
}
