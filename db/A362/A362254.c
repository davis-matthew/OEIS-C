/*
 * A362254  Reciprocal of n modulo largest prime smaller than n.
 * https://oeis.org/A362254
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = the modular inverse of n modulo the largest prime smaller
 *            than n (for n ≥ 3). The inverse is computed via the extended
 *            Euclidean algorithm
 * Range:     u8 n <= 335, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/build_primes.h"

static idx_t *prime_list = NULL;   /* array of primes in ascending order */
static size_t   prime_cnt  = 0;     /* number of primes in prime_list      */
static idx_t    prime_limit = 0;   /* largest n already sieved up to      */

/* Ensure we have primes up to (and including) limit.  */
static void ensure_primes_up_to(idx_t limit)
{
    if (prime_limit >= limit)
        return;

    /* Rebuild the prime sieve.  The old list is freed automatically by
       build_primes (it allocates a new array). */
    if (prime_list)
        free(prime_list);
    build_primes(limit, &prime_list, &prime_cnt);
    prime_limit = limit;
}

/* Return the largest prime < n.  n ≥ 3, so such a prime always exists. */
static idx_t largest_prime_below(idx_t n)
{
    /* The list is ascending; scan backwards.  This is fast enough for the
       expected input sizes (≤ 10 000). */
    for (size_t i = prime_cnt; i > 0; --i) {
        idx_t p = prime_list[i - 1];
        if (p < n)
            return p;
    }
    return 2;  /* never reached for n ≥ 3 */
}

/* Extended Euclidean algorithm returning x ≡ a⁻¹ (mod mod).  The result
   is a number in the range [0, mod‑1].  All arithmetic fits in uint64_t
   using 128‑bit intermediates where necessary. */
static uint64_t inv_mod_uint64(uint64_t a, uint64_t mod)
{
    int64_t t    = 0;
    int64_t newt = 1;
    uint64_t r    = mod;
    uint64_t newr = a % mod;

    while (newr != 0) {
        uint64_t q = r / newr;

        int64_t tmp_t = t - (int64_t)((__int128)q * newt);
        t     = newt;
        newt  = tmp_t;

        uint64_t tmp_r = (uint64_t)((__int128)r - (__int128)q * newr);
        r     = newr;
        newr  = tmp_r;
    }

    /* In the context of this sequence, mod is prime and a < mod,
       so gcd(a, mod) == 1 always holds.  */
    if (t < 0)
        t += mod;
    return (uint64_t)t;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n < 3)
        return 0;                  /* undefined below the offset */

    ensure_primes_up_to(n - 1);     /* we need primes < n */
    idx_t p = largest_prime_below(n);  /* p is a prime < n */

    uint64_t inv = inv_mod_uint64((uint64_t)n, (uint64_t)p);
    return (seq_t)inv;             /* cast back to seq_t (truncation OK) */
}
