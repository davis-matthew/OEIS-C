/*
 * A343974  Even numbers k such that the two sets of primes in the Goldbach
 *          representation of k and k+2 as the sum of two odd primes do not
 *          intersect.
 * https://oeis.org/A343974
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) computed by searching successive even numbers and * keeping
 *            the sequence of even integers k such that the set of * primes
 *            appearing in any Goldbach partition of k and the set of * primes
 *            appearing in any Goldbach partition of k+2 are disjoint. * The
 *            function lazily builds a prime sieve and a...
 * Range:     u8 n <= 15, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/*------------------------------------------------------------*/
/*  Dynamic prime sieve                                         */
/*------------------------------------------------------------*/
static bool *prime = NULL;      /* prime[i] == true  <=>  i is prime   */
static size_t prime_limit = 0;  /* allocated up to prime_limit        */

static void init_sieve(size_t limit)
{
    size_t i, j;
    if (prime) free(prime);
    prime = (bool *)malloc((limit + 1) * sizeof(bool));
    /* init all to true */
    memset(prime, 1, (limit + 1) * sizeof(bool));
    prime[0] = prime[1] = false;
    for (i = 2; i * i <= limit; ++i) {
        if (prime[i]) {
            for (j = i * i; j <= limit; j += i)
                prime[j] = false;
        }
    }
    prime_limit = limit;
}

static void ensure_prime_limit(size_t limit)
{
    if (limit <= prime_limit) return;
    init_sieve(limit);
}

/*------------------------------------------------------------*/
/*  Cache for results                                            */
/*------------------------------------------------------------*/
static seq_t *cache = NULL;     /* array of results       */
static size_t cache_sz = 0;      /* number of stored terms */
static size_t cache_cap = 0;     /* allocated capacity     */

/* make sure cache has at least 'need' slots, expanding if required */
static void ensure_cache_cap(size_t need)
{
    if (cache_cap >= need) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 4;
    while (new_cap < need) new_cap *= 2;
    seq_t *new_arr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = new_arr;
    cache_cap = new_cap;
}

/*------------------------------------------------------------*/
/*  Temporary array to mark primes encountered in a partition */
/*------------------------------------------------------------*/
static bool *used = NULL;       /* used[p] == true if p appears in a partition */
static size_t used_limit = 0;   /* allocated up to used_limit          */

static void ensure_used_limit(size_t limit)
{
    if (limit <= used_limit) return;
    if (used) free(used);
    used = (bool *)malloc((limit + 1) * sizeof(bool));
    used_limit = limit;
}

/*------------------------------------------------------------*/
/*  a(n) : main routine                                       */
/*------------------------------------------------------------*/
static idx_t next_candidate = 6;      /* first even number to test */

seq_t a(idx_t n)
{
    /* Sanity: n must be at least 1 */
    if (n == 0) return (seq_t)0;

    /* Make sure we have a prime sieve big enough for the first candidate */
    ensure_prime_limit((size_t)next_candidate + 2);
    ensure_used_limit(prime_limit);

    /* Build the cache until we have at least n terms */
    while (cache_sz < (size_t)n) {
        /* Ensure the sieve covers next_candidate + 2 */
        size_t need = (size_t)next_candidate + 2;
        if (need > prime_limit) {
            /* Extend sieve and used array */
            size_t new_limit = need * 2;   /* safety factor */
            init_sieve(new_limit);
            ensure_used_limit(prime_limit);
        }

        /* mark primes used in partitions of k = next_candidate */
        memset(used, 0, prime_limit + 1);

        idx_t k = next_candidate;
        idx_t half = k / 2;
        for (idx_t p = 3; p <= half; p += 2) {
            if (prime[p] && prime[k - p]) {
                used[p] = true;
                used[k - p] = true;
            }
        }

        /* Check partitions of k+2 for intersections */
        bool ok = true;
        idx_t kp2 = k + 2;
        idx_t half2 = kp2 / 2;
        for (idx_t p = 3; p <= half2; p += 2) {
            if (prime[p] && prime[kp2 - p]) {
                if (used[p] || used[kp2 - p]) {
                    ok = false;
                    break;
                }
            }
        }

        if (ok) {
            /* Store result */
            ensure_cache_cap(cache_sz + 1);
            cache[cache_sz++] = (seq_t)k;
        }

        /* Move to next even number */
        next_candidate += 2;
    }

    return cache[n - 1];
}
