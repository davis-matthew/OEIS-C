/*
 * A366168  Denominator of the second derivative of the n-th Bernoulli
 *          polynomial B(n,x).
 * https://oeis.org/A366168
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(1)=1; for n>1 a(n)=∏{ prime p ≤ n/(2+(n mod 2)) | p∤n,
 *            p∤(n-1), and digit‑sum base p of (n-1) ≥ p } p
 * Range:     u8 n <= 37, u16 n <= 73, u32 n <= 181, u64 n <= 577,
 *            u128 n <= 2161, big any n
 * Helpers:   build_primes_upto, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  seq_t and idx_t are provided by the build. */
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd.h"          /* for gcd(seq_t, seq_t) if needed */
#include "../utils/build_primes_upto.h" /* for build_primes_upto */

/* static cache of primes up to the maximum limit requested so far */
static uint64_t *cached_primes = NULL;
static size_t  cached_prime_count = 0;
static idx_t   cached_limit = 0;

/* helper: free current cache */
static void free_cache(void) {
    if (cached_primes) {
        free(cached_primes);
        cached_primes = NULL;
        cached_prime_count = 0;
        cached_limit = 0;
    }
}

seq_t a(idx_t n) {
    if (n <= 1) return (seq_t)1;

    /* limit for prime search: floor(n / (2 + n%2)) */
    idx_t limit = n / (2 + (n & 1));

    /* rebuild prime cache if necessary */
    if (cached_primes == NULL || limit > cached_limit) {
        free_cache();
        /* build_primes_upto allocates an array of uint64_t and sets cnt_out */
        build_primes_upto(limit, &cached_primes, &cached_prime_count);
        cached_limit = limit;
    }

    seq_t result = (seq_t)1;
    idx_t nm1 = n - 1;

    for (size_t i = 0; i < cached_prime_count; ++i) {
        uint64_t p = cached_primes[i];
        if (p > limit) break;
        if (p == 2) continue;                /* sequence is odd */
        if ((n % p) == 0) continue;          /* gcd(p,(n)_2)!=1 */
        if (((n - 1) % p) == 0) continue;    /* gcd(p,(n)_2)!=1 */

        /* compute digit sum of n-1 in base p */
        idx_t x = nm1;
        idx_t sum = 0;
        while (x > 0) {
            sum += x % p;
            x /= p;
        }
        if (sum >= p) {
            result *= (seq_t)p;
        }
    }

    return result;
}
