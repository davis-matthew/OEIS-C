/*
 * A296058  Numbers k such that floor((3*k - 1)/2) is prime.
 * https://oeis.org/A296058
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((2*prime(n+1)+2)/3)
 * Range:     u8 n <= 74, u16 n <= 9438, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include "../utils/build_primes_upto.h"

/* Global prime cache (0‑based, prime[0] = 2) */
static uint64_t *primes_cache = NULL;
static size_t primes_cnt    = 0;
static idx_t  sieve_limit   = 0;

/* Forward declaration of helper */
static void ensure_primes(idx_t n);

/* Sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* undefined for offset 0 */

    /* Ensure we have prime(n+1) cached */
    ensure_primes(n);

    uint64_t p   = primes_cache[n];         /* prime(n+1) */
    uint64_t val = (p * 2ULL + 2ULL) / 3ULL;/* floor((2p+2)/3) */
    return (seq_t)val;
}

/* Make sure primes_cache holds at least (n+1)-th prime */
static void ensure_primes(idx_t n)
{
    idx_t needed = n + 1;       /* number of primes required */

    /* Return if already satisfied */
    if (primes_cnt >= (size_t)needed)
        return;

    /* Starting upper limit for the sieve */
    idx_t limit = (sieve_limit > 0) ? sieve_limit : 16;

    /* Grow the sieve until we have enough primes */
    while (true) {
        /* Free any previous allocation */
        if (primes_cache) {
            free(primes_cache);
            primes_cache = NULL;
        }

        /* Build primes up to the current limit */
        build_primes_upto(limit, &primes_cache, &primes_cnt);
        sieve_limit = limit;

        if (primes_cnt >= (size_t)needed)
            break;

        /* Double the limit for the next iteration */
        limit <<= 1;
    }
}
