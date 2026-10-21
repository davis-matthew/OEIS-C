/*
 * A245077  Largest k such that the smallest prime satisfying Goldbach's
 *          conjecture is less than or equal to (2n)^(1/k).
 * https://oeis.org/A245077
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor( log(2*n) / log(L(n)) ) where L(n) = the smallest
 *            prime p with 2*n - p prime
 * Range:     u8 n <= 1702, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime.h"
#include "../utils/build_primes.h"

static idx_t *primes = NULL;           /* array of primes up to the current limit */
static size_t prime_cnt = 0;           /* number of primes stored */
static idx_t prime_limit = 0;          /* current maximum prime limit used */

/* Ensure that 'primes' contains all primes ≤ 'limit'. */
static void
ensure_primes_upto(idx_t limit)
{
    if (limit <= prime_limit)
        return;
    if (primes)
        free(primes);
    build_primes(limit, &primes, &prime_cnt);
    prime_limit = limit;
}

/* Return the smallest prime p that participates in a Goldbach partition of 2*n. */
static seq_t
smallest_goldbach_prime(idx_t n)
{
    idx_t two_n = n + n;                /* 2*n, fits safely in idx_t for the problem size */
    ensure_primes_upto(two_n);

    for (size_t i = 0; i < prime_cnt; ++i) {
        idx_t p = primes[i];
        if (p > two_n)
            break;
        if (is_prime(two_n - p))
            return (seq_t)p;           /* first such prime is the smallest */
    }
    /* In theory this point should never be reached for n ≥ 2. */
    return (seq_t)2;
}

/* Main function that returns a(n).  Called with n ≥ 2, but handles smaller n as well. */
seq_t
a(idx_t n)
{
    if (n < 2)  /* behavior undefined for n < 2, but we return 0 to satisfy the type system */
        return (seq_t)0;

    /* Compute L(n) and 2*n */
    seq_t L = smallest_goldbach_prime(n);
    seq_t two_n = (seq_t)n * (seq_t)2;

    /* Find the largest k such that L^k ≤ 2*n.  k is small (≤ ~15 for n ≤ 10000). */
    seq_t power = 1;
    seq_t k = 0;
    while (power <= two_n / L) {
        power *= L;
        ++k;
    }
    return (seq_t)k;
}
