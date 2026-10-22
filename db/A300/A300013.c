/*
 * A300013  a(n) is the number of primes p such that both 2n-p and
 *          2n+2-nextprime(p) are prime numbers.
 * https://oeis.org/A300013
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   build_primes_upto, is_prime, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Formula (OEIS A300013): a(n) equals the number of primes p such that
   both 2n−p and 2n+2−nextprime(p) are prime. */
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"
#include "../utils/is_prime.h"
#include "../utils/next_prime.h"

static uint64_t *primes = NULL;
static size_t prime_cnt = 0;
static idx_t prime_limit = 0;

/* Ensure that all primes up to 'limit' are available in the global 'primes' array. */
static void ensure_primes(idx_t limit) {
    if (limit <= prime_limit)
        return;
    free(primes);
    build_primes_upto(limit, &primes, &prime_cnt);
    prime_limit = limit;
}

seq_t a(idx_t n) {
    if (n == 0)   /* safety for out‑of‑range calls */
        return 0;

    idx_t two_n = 2 * n;
    idx_t two_n_plus2 = two_n + 2;

    /* A safety margin to guarantee next_prime(p) can be found */
    ensure_primes(two_n_plus2 + 100);

    seq_t count = 0;

    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = primes[i];

        /* Skip primes that would make (2n - p) < 2 */
        if (p > two_n - 2)
            break;

        /* 2n - p must be prime */
        if (!is_prime(two_n - p))
            continue;

        uint64_t nextp = next_prime(p);
        if (nextp > two_n_plus2)
            continue;   /* would produce negative value */

        uint64_t val = two_n_plus2 - nextp;
        if (val >= 2 && is_prime(val))
            ++count;
    }

    return count;
}
