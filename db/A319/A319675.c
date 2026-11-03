/*
 * A319675  Sum of digits of prime(n) and digits of prime(n+1).
 * https://oeis.org/A319675
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = digit_sum(prime(n)) + digit_sum(prime(n+1))
 * Range:     u8 n <= 53, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Validated: partial, pass, 66 terms through n = 66 (2026-11-01)
 * Helpers:   build_primes_upto, digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include "../utils/build_primes_upto.h"
#include "../utils/digit_sum.h"

static size_t _prime_limit_for_idx(idx_t n) {
    /* Rough upper bound for the n-th prime using n*(log n + log log n) + 3 */
    if (n < 6) return 15; /* first 6 primes < 15 */
    double nn = (double)n;
    double bound = nn * (log(nn) + log(log(nn))) + 3.0;
    return (size_t)bound;
}

/* static cache for primes */
static uint64_t *prime_cache = NULL;   /* array of primes in ascending order */
static size_t prime_cache_size = 0;    /* number of primes stored */
static idx_t   max_cached_idx = 0;     /* largest index for which we have primes (1‑based) */

/* Rebuild prime cache to contain at least needed_idx primes (1‑based) */
static void ensure_primes_up_to(idx_t needed_idx)
{
    /* already sufficient? */
    if (needed_idx <= max_cached_idx)
        return;

    /* find an appropriate upper bound for the needed prime */
    idx_t limit = _prime_limit_for_idx(needed_idx);

    /* free old cache if any */
    if (prime_cache)
        free(prime_cache);

    /* build primes up to 'limit' */
    uint64_t *primes_out = NULL;
    size_t     count_out  = 0;
    build_primes_upto(limit, &primes_out, &count_out);

    /* store new cache; assume it contains at least needed_idx primes */
    prime_cache       = primes_out;
    prime_cache_size  = count_out;
    max_cached_idx    = (idx_t)prime_cache_size;   /* 1‑based count */
}

/*  Returns a(n) for n >= 1. */
seq_t a(idx_t n)
{
    if (n == 0)   /* safety for out‑of‑range calls */
        return 0;

    /* ensure we have primes up to prime(n+1) */
    ensure_primes_up_to(n + 1);

    /* indices in zero‑based array */
    uint64_t p1 = prime_cache[n - 1];   /* prime(n)   */
    uint64_t p2 = prime_cache[n];       /* prime(n+1) */

    /* compute digit sums and return the result */
    seq_t s1 = digit_sum((seq_t)p1);
    seq_t s2 = digit_sum((seq_t)p2);
    return s1 + s2;
}
