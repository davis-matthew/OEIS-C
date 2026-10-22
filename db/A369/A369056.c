/*
 * A369056  Numbers k of the form 4m+3 for which there is no representation as
 *          a sum (p*q + p*r + q*r) with three odd primes p <= q <= r.
 * https://oeis.org/A369056
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n‑th smallest integer k ≡ 3 (mod 4) that cannot be
 *            expressed as p*q + p*r + q*r with odd primes p ≤ q ≤ r.
 *            Computation performed by incremental search with a precomputed
 *            prime sieve. This is not a closed form but follows exactly the
 *            definition
 * Range:     u8 n <= 32, u16 n <= 3737, u32 n <= 9503, u64 n <= 9503,
 *            u128 n <= 9503, big any n
 * Helpers:   build_primes_upto, isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include "../utils/build_primes_upto.h"
#include "../utils/isPrimeGeneric.h"

/* Local helper to ensure that the prime list contains all primes
   up to at least `needed`. If not, the sieve is rebuilt with a larger
   limit (doubling until sufficient). */
static void ensure_primes_up_to(idx_t needed, uint64_t **primes,
                                size_t *prime_cnt, idx_t *prime_limit)
{
    if (needed <= *prime_limit)
        return;
    idx_t new_limit = *prime_limit;
    while (new_limit < needed)
        new_limit <<= 1;
    free(*primes);                    /* discard old table */
    build_primes_upto(new_limit, primes, prime_cnt);
    *prime_limit = new_limit;
}

/* Test whether `k` (odd, k≡3 mod 4) can be written as
   p*q + p*r + q*r with odd primes p ≤ q ≤ r. */
static bool isRep(uint64_t k, const uint64_t *primes, size_t prime_cnt,
                 idx_t *prime_limit)
{
    /* Upper bound on p: 3*p^2 ≤ k  →  p ≤ sqrt(k/3). */
    uint64_t p_limit = (uint64_t)floor(sqrt((double)k / 3.0));
    ensure_primes_up_to(p_limit, (uint64_t**)&primes, &prime_cnt, prime_limit);

    for (size_t ip = 0; ip < prime_cnt; ++ip) {
        uint64_t p = primes[ip];
        if (p == 2)          /* representation requires odd primes only */
            continue;
        if (p > p_limit)
            break;
        for (size_t iq = ip; iq < prime_cnt; ++iq) {
            uint64_t q = primes[iq];
            __uint128_t pq = (__uint128_t)p * q;
            if (pq >= k)
                break;        /* further q only increases pq */
            __uint128_t rem = (__uint128_t)k - pq;
            uint64_t d = p + q;
            if (rem % d != 0)
                continue;
            uint64_t r = (uint64_t)(rem / d);
            if (r < q)        /* r decreases with increasing q */
                break;
            if (r <= 2 || (r & 1ULL) == 0)
                continue;    /* r must be an odd prime >2 */
            if (isPrimeGeneric((seq_t)r))
                return true; /* found a valid triple */
        }
    }
    return false;
}

seq_t a(idx_t n)
{
    /* Static state shared across calls */
    static bool initialized = false;
    static uint64_t *primes = NULL;
    static size_t prime_cnt = 0;
    static idx_t prime_limit = 0;

    static seq_t *nonrep = NULL;
    static size_t nonrep_cnt = 0;
    static size_t nonrep_cap = 0;

    static uint64_t candidate = 3;   /* first integer ≡3 (mod 4) */

    if (!initialized) {
        /* Initial prime sieve (1 000 000 is ample for n up to 10 000). */
        prime_limit = 1000000;
        build_primes_upto(prime_limit, &primes, &prime_cnt);

        nonrep_cap = 1024;
        nonrep = (seq_t*)calloc(nonrep_cap, sizeof(seq_t));

        initialized = true;
    }

    /* Generate terms until we have at least n of them. */
    while (nonrep_cnt < n) {
        if ((candidate & 3ULL) == 3ULL) {          /* candidate ≡ 3 (mod 4) */
            uint64_t k = candidate;
            if (!isRep(k, primes, prime_cnt, &prime_limit)) {
                /* Store the non‑representable value. */
                if (nonrep_cnt >= nonrep_cap) {
                    nonrep_cap <<= 1;
                    nonrep = (seq_t*)realloc(nonrep,
                                             nonrep_cap * sizeof(seq_t));
                }
                nonrep[nonrep_cnt++] = (seq_t)k;
            }
        }
        ++candidate;
    }

    return nonrep[n - 1];
}
