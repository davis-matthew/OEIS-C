/*
 * A398314  a(n) = n * A002110(pi(gpf(n))), where pi = A000720 and gpf(n) is
 *          the greatest prime factor of n.
 * https://oeis.org/A398314
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n * A002110(pi(gpf(n)))
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 22, u64 n <= 46, u128 n <= 100,
 *            big any n
 * Helpers:   build_primes_upto, gpf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) = n * (product of all primes <= gpf(n))   (n ≥ 1)  */

/*  The sequence is defined for n ≥ 1. No special handling for n < 1 is
 *  required except returning 0 for safety.  The implementation builds
 *  a list of primes up to the greatest prime factor found so far and
 *  multiplies the appropriate subset of those primes for each call.  */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* Utility functions provided by the library */
#include "../utils/gpf.h"          /* uint64_t gpf(uint64_t) */
#include "../utils/build_primes_upto.h"   /* build_primes_upto(idx_t, uint64_t**, size_t*) */

/* Static state: list of primes and how many we have. */
static uint64_t *primes = NULL;      /* array of primes */
static size_t   prime_cnt = 0;      /* number of primes in the array */
static idx_t    prime_limit = 0;    /* maximum prime value currently cached */

/* Ensure primes up to at least LIMIT are available. */
static void
ensure_primes_up_to(idx_t limit)
{
    if (limit <= prime_limit) return;           /* already have enough */
    /* build_primes_upto allocates a new array and assigns it to *primes_out */
    build_primes_upto(limit, &primes, &prime_cnt);
    prime_limit = limit;
}

/* Main sequence function */
seq_t
a(idx_t n)
{
    if (n == 0) return 0;                     /* safety guard; offset is 1 */

    /* Greatest prime factor of n (unsigned integer). */
    uint64_t g = gpf((uint64_t)n);

    /* Make sure we have all primes up to that factor. */
    ensure_primes_up_to(g);

    /* Count how many primes are ≤ g. */
    size_t pi = 0;
    while (pi < prime_cnt && primes[pi] <= g) ++pi;

    /* Compute the result: n multiplied by the product of the first pi primes. */
    seq_t result = (seq_t)n;
    for (size_t j = 0; j < pi; ++j) {
        result *= (seq_t)primes[j];
    }
    return result;
}
