/*
 * A370010  a(n) is the greatest prime less than 4*prime(n).
 * https://oeis.org/A370010
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the greatest prime strictly less than 4 × prime(n)
 * Range:     u8 n <= 18, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   _prime_limit_for_idx, build_primes_upto, prev_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function uses the supplied utilities for prime generation and
 * the helper `prev_prime` which returns the largest prime < a given bound. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Utility headers (one per function name) */
#include "../utils/_prime_limit_for_idx.h"
#include "../utils/build_primes_upto.h"
#include "../utils/prev_prime.h"

/* Cache of the primes computed so far (1‑based index) */
static idx_t    cached_num_primes = 0;   /* number of primes stored */
static uint64_t *cached_primes   = NULL; /* array of primes, 0‑based */

static void
ensure_primes_upto(idx_t n)
{
    if (n <= cached_num_primes)
        return;

    /* Get an upper bound for the n‑th prime */
    idx_t limit = _prime_limit_for_idx(n);

    /* Allocate and build primes up to that limit */
    uint64_t *new_primes = NULL;
    size_t    new_cnt    = 0;
    build_primes_upto(limit, &new_primes, &new_cnt);

    /* Replace the old cache with the new one */
    free(cached_primes);
    cached_primes   = new_primes;
    cached_num_primes = (idx_t)new_cnt;
}

seq_t
a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;   /* undefined for n < 1, 0 is an arbitrary placeholder */

    /* Ensure we have at least the first n primes cached */
    ensure_primes_upto(n);

    /* nth prime (1‑based index) */
    uint64_t p = cached_primes[n - 1];

    /* Bound is 4 × p; we need the greatest prime strictly less than this */
    uint64_t bound = 4ull * p;

    /* prev_prime returns the largest prime < bound */
    uint64_t result = prev_prime(bound);

    return (seq_t)result;
}
