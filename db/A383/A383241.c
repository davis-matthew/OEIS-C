/*
 * A383241  a(n) = p(n)*p(n+1)*(p(n+1) - p(n)) - 1, where p(n) = prime(n).
 * https://oeis.org/A383241
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = p(n) * p(n+1) * (p(n+1) - p(n)) - 1, where p(k) is the
 *            k‑th prime
 * Range:     u8 n <= 3, u16 n <= 23, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

#include "../utils/build_primes_upto.h"   /* provides build_primes_upto */

static uint64_t *prime_cache = NULL;   /* array of primes, 1‑based primes[0] = 2   */
static size_t  prime_count   = 0;      /* number of primes stored */
static idx_t   sieve_limit   = 0;      /* upper bound that was sieved */

/* Ensure that prime_cache contains at least 'n' primes (n≥1) */
static void
ensure_primes(idx_t n)
{
    if (prime_count >= (size_t)n)
        return;

    /* Approximate upper bound: p_n < n*(20) for n≥5, reasonable for n≈10 000. */
    idx_t limit = n * 20 + 50;
    if (limit <= sieve_limit)
        limit = sieve_limit * 2 + 10;    /* double until enough */

    uint64_t *new_primes = NULL;
    size_t   new_cnt   = 0;

    build_primes_upto(limit, &new_primes, &new_cnt);

    free(prime_cache);          /* old allocation – allowed to free */
    prime_cache = new_primes;
    prime_count = new_cnt;
    sieve_limit = limit;
}

seq_t
a(idx_t n)
{
    if (n < 1)
        return 0;                 /* undefined for n=0; return 0 as placeholder */

    ensure_primes(n + 1);          /* need p(n) and p(n+1) */

    seq_t p1  = (seq_t)prime_cache[n - 1];   /* p(n) */
    seq_t p2  = (seq_t)prime_cache[n];       /* p(n+1) */
    seq_t diff = p2 - p1;
    seq_t prod = p1 * p2 * diff;

    return prod - (seq_t)1;
}
