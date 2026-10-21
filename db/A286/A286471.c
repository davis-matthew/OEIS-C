/*
 * A286471  If n is noncomposite, then a(n) = 0, otherwise 1 + difference
 *          between indices of the two smallest (not necessarily distinct)
 *          prime factors of n.
 * https://oeis.org/A286471
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 0 if n is noncomposite, otherwise a(n) = 1 + (index of
 *            second smallest prime factor) – (index of smallest prime factor)
 * Range:     u8 n <= 3237, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _floor_isqrt_u64, build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* NOTE: uses the helper functions build_primes_upto and _floor_isqrt_u64
   from the utils library. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"
#include "../utils/_floor_isqrt_u64.h"

/* static storage for a growing list of primes */
static idx_t primes_limit = 0;          /* current sieve limit          */
static size_t primes_cnt   = 0;          /* number of primes stored       */
static uint64_t *primes     = NULL;     /* array of primes (ascending)   */

/* ensure that the primes table contains all primes ≤ limit */
static void ensure_primes_upto(idx_t limit)
{
    if (primes_limit >= limit) return;

    /* free old table if any */
    if (primes) free(primes);

    size_t cnt = 0;
    uint64_t *new_primes = NULL;
    build_primes_upto(limit, &new_primes, &cnt);

    primes = new_primes;
    primes_cnt = cnt;
    primes_limit = limit;
}

/* return the smallest prime factor of n (or n if n is prime) */
static uint64_t spf(idx_t n)
{
    if (n <= 1) return n;

    idx_t sqrt_n = _floor_isqrt_u64(n);
    if (primes_limit < sqrt_n) ensure_primes_upto(sqrt_n);

    for (size_t i = 0; i < primes_cnt; ++i) {
        uint64_t p = primes[i];
        if (p > sqrt_n) break;
        if (n % p == 0) return p;
    }
    return n; /* n is prime */
}

/* find the (1‑based) index of a prime p in the primes table */
static idx_t prime_index(uint64_t p)
{
    if (p > primes_limit) ensure_primes_upto(p);

    for (size_t i = 0; i < primes_cnt; ++i) {
        if (primes[i] == p) return (idx_t)(i + 1);
    }
    /* p must be prime and present; should never reach here */
    return 0;
}

seq_t a(idx_t n)
{
    if (n < 2) return 0;               /* 1 is noncomposite */

    uint64_t s = spf(n);
    if (s == n) return 0;              /* n is prime (or 1) */

    uint64_t p1 = s;                    /* smallest prime factor   */
    uint64_t m  = n / p1;               /* remaining cofactor      */
    uint64_t p2 = spf(m);               /* second smallest pf      */

    /* ensure primes table covers the needed primes for indexing */
    idx_t maxpf = (p1 > p2 ? p1 : p2);
    if (primes_limit < maxpf) ensure_primes_upto(maxpf);

    idx_t i1 = prime_index(p1);
    idx_t i2 = prime_index(p2);

    return (seq_t)(1 + (i2 - i1));
}
