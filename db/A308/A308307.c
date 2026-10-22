/*
 * A308307  Number of integer-sided triangles with perimeter n and at least
 *          one side length that is not semiprime.
 * https://oeis.org/A308307
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A005044(n) – A308069(n) where A005044(n) is the number of
 *            integer‑sided triangles with perimeter n and A308069(n) the
 *            number of such triangles all of whose sides are semiprime. This
 *            implementation enumerates the triples efficiently using the
 *            known bounds to achieve O(n^2) dominance, which...
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/build_primes_upto.h"

/* Cached prime list up to `prime_limit` */
static uint64_t *primes = NULL;   /* list of primes */
static size_t   prime_cnt = 0;    /* number of primes */
static idx_t    prime_limit = 0;  /* maximum prime stored */

/* Cached semiprime flag array up to `semiprime_limit` */
static bool *semiprime_cache = NULL;
static idx_t semiprime_limit = 0;

/* Re‑allocate the prime list so it contains all primes ≤ limit. */
static void ensure_prime_list(idx_t limit)
{
    if (limit <= prime_limit) return;
    /* free previous list */
    free(primes);
    build_primes_upto(limit, &primes, &prime_cnt);
    prime_limit = limit;
}

/* Build a cache indicating whether each integer ≤ limit is a semiprime. */
static void build_semiprime_cache(idx_t limit)
{
    if (semiprime_cache) free(semiprime_cache);
    semiprime_cache = (bool *)calloc((size_t)limit + 1U, sizeof(bool));
    semiprime_limit = limit;

    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = primes[i];
        if (p > limit) break;
        for (size_t j = i; j < prime_cnt; ++j) {
            uint64_t q = primes[j];
            uint64_t prod = p * q;
            if (prod > limit) break;
            semiprime_cache[(size_t)prod] = true;
        }
    }
}

/* Return true iff x is a semiprime. */
static inline bool is_semiprime(seq_t x)
{
    if (x > semiprime_limit) return false;
    return semiprime_cache[(size_t)x];
}

seq_t a(idx_t n)
{
    /* Ensure prime list and cache are large enough. */
    ensure_prime_list(n);
    if (n > semiprime_limit) build_semiprime_cache(n);

    if (n < 3) return 0;  /* no integer triangle exists */

    seq_t total = 0;     /* all triangles */
    seq_t semitot = 0;   /* all sides semiprime */

    for (idx_t a = 1; a <= n / 3; ++a) {
        /* b runs from max(a, floor(n/2 - a)+1) to floor((n-a)/2). */
        idx_t b_min = a;
        idx_t tmp = (n / 2) - a;
        if (tmp >= a) b_min = tmp + 1;

        idx_t b_max = (n - a) / 2;
        if (b_min > b_max) continue;

        for (idx_t b = b_min; b <= b_max; ++b) {
            idx_t c = n - a - b;          /* automatically satisfies b ≤ c */
            ++total;
            if (is_semiprime(a) && is_semiprime(b) && is_semiprime(c))
                ++semitot;
        }
    }

    return total - semitot;
}
