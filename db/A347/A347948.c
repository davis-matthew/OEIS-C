/*
 * A347948  E.g.f.: 1 / (1 - Sum_{k>=1} x^prime(k) / prime(k)!).
 * https://oeis.org/A347948
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; for n > 0 a(n) = Σ_{p prime ≤ n} binom(n,p) * a(n-p)
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 14, u64 n <= 22, u128 n <= 23,
 *            big any n
 * Validated: partial, pass, 24 terms through n = 23 (2026-11-03)
 * Helpers:   binomial, build_primes, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Required utilities:
   - binom for binomial coefficients
   - ensure_cache to allocate the result cache
   - build_primes to generate the prime list up to a given limit */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/binomial.h"
#include "../utils/build_primes.h"
#include "../utils/mem.h"

/* Make sure the cache is large enough */
static void ensure_cache(idx_t n, seq_t **cache_ptr, size_t *cap_ptr) {
    seq_t *cache = *cache_ptr;
    size_t cap = *cap_ptr;
    if (cap <= (size_t)n) {
        size_t new_cap = cap ? (cap << 1) : 16;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp && new_cap > 0) {
            /* Allocation failed – abort the program. */
            exit(1);
        }
        if (tmp) {
            /* Initialize newly allocated slots to zero. */
            if (!cache) {
                for (size_t i = 0; i < new_cap; ++i)
                    tmp[i] = 0;
            } else {
                for (size_t i = cap; i < new_cap; ++i)
                    tmp[i] = 0;
            }
            *cache_ptr = tmp;
            *cap_ptr = new_cap;
        }
    }
}

seq_t a(idx_t n)
{
    /* --- static state for memoisation and prime handling --- */
    static seq_t *cache = NULL;              /* cached terms a(0) … a(... )       */
    static size_t cache_cap = 0;             /* current cache size                */
    static idx_t cache_computed = 0;         /* highest index that has been computed */
    static bool cache_initialised = false;   /* cache[0] set?                      */

    static idx_t *prime_list = NULL;         /* primes ≤ current prime_limit       */
    static size_t prime_count = 0;           /* number of primes in prime_list     */
    static idx_t prime_limit = 0;            /* largest n for which prime_list is accurate */

    /* --- prepare cache ------------------------------------ */
    ensure_cache(n, &cache, &cache_cap);

    if (!cache_initialised) {
        cache[0] = (seq_t)1;                 /* a(0) = 1                         */
        cache_computed = 0;
        cache_initialised = true;
    }

    /* --- generate primes up to n if needed ----------------- */
    if (prime_limit < n) {
        if (prime_list != NULL)
            free(prime_list);
        build_primes(n, &prime_list, &prime_count);
        prime_limit = n;
    }

    /* --- compute missing terms from cache_computed+1 up to n --- */
    for (idx_t m = cache_computed + 1; m <= n; ++m) {
        seq_t sum = (seq_t)0;
        for (size_t i = 0; i < prime_count && prime_list[i] <= m; ++i) {
            idx_t p = prime_list[i];
            seq_t bin = binomial(m, p);           /* binomial coefficient C(m,p)   */
            seq_t prod = bin * cache[m - p];   /* term contribution             */
            sum += prod;                       /* accumulate                 */
        }
        cache[m] = sum;
    }
    cache_computed = n;

    return cache[n];
}
