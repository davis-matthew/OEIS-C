/*
 * A355661  Largest number of children of any vertex in the rooted tree with
 *          Matula-Goebel number n.
 * https://oeis.org/A355661
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = max(bigomega(n), {a(primepi(p)) | p prime factor of n})
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   bigomega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/bigomega.h"

/* Cached data structures */
static seq_t       *a_cache   = NULL;   /* a(0)..a(n) */
static unsigned char *is_prime = NULL;   /* sieve 0/1 */
static idx_t       *pi_arr    = NULL;   /* pi(i) for 0..n */
static uint64_t    *prime_list = NULL;   /* list of primes <= n */
static size_t       prime_count = 0;     /* number of primes stored */
static idx_t        sieve_limit = 0;     /* maximum index currently sieved */
static idx_t        cache_upto   = 0;     /* largest n for which a_cache is valid */

/* Build sieve, pi array and prime list up to limit (inclusive) */
static void build_sieve(idx_t limit)
{
    /* sieve of Eratosthenes */
    is_prime = (unsigned char *)realloc(is_prime, (limit + 1) * sizeof(unsigned char));
    for (idx_t i = 0; i <= limit; ++i) is_prime[i] = 1;
    if (limit >= 0) is_prime[0] = 0;
    if (limit >= 1) is_prime[1] = 0;
    for (idx_t i = 2; i * i <= limit; ++i) {
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= limit; j += i) is_prime[j] = 0;
        }
    }

    /* prime counting function */
    pi_arr = (idx_t *)realloc(pi_arr, (limit + 1) * sizeof(idx_t));
    idx_t cnt = 0;
    for (idx_t i = 0; i <= limit; ++i) {
        if (is_prime[i]) ++cnt;
        pi_arr[i] = cnt;
    }

    /* build prime list */
    prime_list = (uint64_t *)realloc(prime_list, (cnt + 1) * sizeof(uint64_t));
    prime_count = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        if (is_prime[i]) prime_list[prime_count++] = i;
    }

    sieve_limit = limit;
}

/* Ensure that a_cache[0..n] and supporting tables exist up to n */
static void ensure_cache(idx_t n)
{
    if (n <= cache_upto) return;               /* already prepared */

    /* extend sieve and prime list if necessary */
    if (sieve_limit < n) build_sieve(n);

    /* enlarge a_cache array */
    a_cache = (seq_t *)realloc(a_cache, (n + 1) * sizeof(seq_t));
    if (cache_upto == 0) {                     /* first run */
        a_cache[0] = 0;
        a_cache[1] = 0;
    }

    /* compute a(k) for k = cache_upto+1 .. n */
    for (idx_t i = cache_upto + 1; i <= n; ++i) {
        seq_t best = bigomega(i);              /* base value */

        uint64_t t = (uint64_t)i;              /* factorisation working copy */

        /* trial division using the precomputed prime list */
        for (size_t pi = 0; pi < prime_count; ++pi) {
            uint64_t p = prime_list[pi];
            if (p * p > t) break;
            if (t % p == 0) {
                seq_t cand = a_cache[pi_arr[p]];            /* a(primepi(p)) */
                if (cand > best) best = cand;
                while (t % p == 0) t /= p;
            }
        }

        /* remaining prime factor, if any */
        if (t > 1) {
            seq_t cand = a_cache[pi_arr[t]];
            if (cand > best) best = cand;
        }

        a_cache[i] = best;
    }

    cache_upto = n;
}

/* Public function required by the test harness */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n=0, but keep safe */
    ensure_cache(n);
    return a_cache[n];
}
