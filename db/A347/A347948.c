/*
 * A347948  E.g.f.: 1 / (1 - Sum_{k>=1} x^prime(k) / prime(k)!).
 * https://oeis.org/A347948
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; for n > 0 a(n) = Σ_{p prime ≤ n} binom(n,p) * a(n-p)
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 14, u64 n <= 22, u128 n <= 23,
 *            big any n
 * Helpers:   binom, build_primes, ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Required utilities:
   - binom for binomial coefficients
   - ensure_cache to allocate the result cache
   - build_primes to generate the prime list up to a given limit */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/binom.h"
#include "../utils/ensure_cache.h"
#include "../utils/build_primes.h"

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
            seq_t bin = binom(m, p);           /* binomial coefficient C(m,p)   */
            seq_t prod = bin * cache[m - p];   /* term contribution             */
            sum += prod;                       /* accumulate                 */
        }
        cache[m] = sum;
    }
    cache_computed = n;

    return cache[n];
}
