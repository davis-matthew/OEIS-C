/*
 * A374474  a(n) = 1 if the least prime dividing the sum of divisors of n is
 *          equal to the least prime not dividing n, otherwise a(n) = 0.
 * https://oeis.org/A374474
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if the least prime dividing the sum of divisors of n
 *            equals the least prime not dividing n, else a(n) = 0. n starts
 *            at 1
 * Range:     u8 n <= 8204, u16 n <= 8204, u32 n <= 8204, u64 n <= 8204,
 *            u128 n <= 8204, big any n
 * Helpers:   build_primes, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/build_primes.h"   /* build_primes(limit, &primes, &cnt) */
#include "../utils/sigma_of.h"      /* sigma_of(n) -> seq_t */

static idx_t *primes = NULL;
static size_t prime_cnt = 0;
static bool primes_built = false;

/* initialise the list of primes once */
static void init_primes(void) {
    if (!primes_built) {
        /* enough for all n <= 10^4; primes up to 100000 cover all needed cases */
        idx_t limit = 100000;
        build_primes(limit, &primes, &prime_cnt);
        primes_built = true;
    }
}

/* least prime that does NOT divide n */
static uint64_t least_nondividing_prime(idx_t n) {
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = (uint64_t)primes[i];
        if (n % p != 0)
            return p;
    }
    /* should not be reached for n >= 1 */
    return 0;
}

/* least prime dividing the sum of divisor of n */
static uint64_t least_dividing_sigma(idx_t n) {
    seq_t sigma_seq = sigma_of(n);
    uint64_t sigma = (uint64_t)sigma_seq;   /* safe for n up to 10⁴ */

    if (sigma == 1)   /* sigma(1) = 1 has no prime divisor */
        return 0;

    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = (uint64_t)primes[i];
        if (p * p > sigma)
            break;
        if (sigma % p == 0)
            return p;
    }
    /* none found up to sqrt(sigma) => sigma itself is prime */
    return sigma;
}

seq_t a(idx_t n) {
    init_primes();

    uint64_t lp_sigma   = least_dividing_sigma(n);
    uint64_t lp_nondiv  = least_nondividing_prime(n);

    return (seq_t)(lp_sigma == lp_nondiv);
}
