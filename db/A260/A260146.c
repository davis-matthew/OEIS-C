/*
 * A260146  Number of positive divisors of hyperfactorial(n).
 * https://oeis.org/A260146
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 16, u64 n <= 32, u128 n <= 66,
 *            big any n
 * Helpers:   build_primes, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A260146 – Number of positive divisors of the hyperfactorial
 *  H(n) = ∏_{k=1}^n k^k  (H(0)=1).
 *  a(n) = τ( H(n) ).
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/build_primes.h"
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* static state for incremental computation */
    static idx_t  max_n        = 0;           /* largest n already computed   */
    static seq_t *cache        = NULL;        /* a_cache[0..max_n]            */
    static size_t cache_cap    = 0;           /* current cache capacity       */
    static uint64_t *primes    = NULL;        /* primes ≤ max_n                */
    static size_t   prime_cnt   = 0;           /* number of primes stored      */
    static idx_t *exp_arr       = NULL;       /* exponent of each prime       */
    static seq_t divisor_count   = 0;          /* divisor count of H(max_n)    */

    /* initialise for n = 0 */
    if (cache == NULL) {
        cache_cap = 1;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;                  /* out of memory */
        divisor_count = 1;                     /* H(0) = 1 has one divisor   */
        cache[0] = divisor_count;
    }

    /* if the requested n has already been computed */
    if (n <= max_n) return cache[n];

    /* extend the cache to hold values up to n */
    if (n + 1 > cache_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;/* out of memory */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* extend the prime list only if necessary */
    if (n >= 2 && (size_t)n > prime_cnt) {
        uint64_t *old_primes = primes;
        size_t old_prime_cnt = prime_cnt;

        /* build new prime list up to n */
        build_primes((idx_t)n, &primes, &prime_cnt);
        free(old_primes);

        /* grow exp_arr to match new prime count */
        idx_t *old_exp_arr = exp_arr;
        exp_arr = (idx_t *)realloc(exp_arr, prime_cnt * sizeof(idx_t));
        if (!exp_arr) return 0;
        for (size_t i = old_prime_cnt; i < prime_cnt; ++i) exp_arr[i] = 0;
    }

    /* temporary arrays for factorisation */
    uint64_t factors[64];
    uint64_t exps[64];
    size_t  factor_cnt;

    /* compute values for all n greater than max_n up to the target n */
    for (idx_t cur = max_n + 1; cur <= n; ++cur) {
        /* factorise cur */
        factor_u64((uint64_t)cur, factors, exps, &factor_cnt);

        /* update exponent of each prime in H(cur) */
        for (size_t fi = 0; fi < factor_cnt; ++fi) {
            uint64_t p   = factors[fi];
            idx_t    t   = exps[fi];          /* exponent of p in cur      */
            idx_t  delta = cur * t;           /* added exponent to H(cur)  */

            /* locate the index of p in primes[] */
            idx_t idx = (idx_t)-1;
            for (size_t i = 0; i < prime_cnt; ++i) {
                if (primes[i] == p) { idx = (idx_t)i; break; }
            }
            /* idx should always be found */
            if (idx == (idx_t)-1) continue;

            idx_t old_e = exp_arr[idx];
            idx_t new_e = old_e + delta;
            /* update divisor count: τ = τ / (old_e+1) * (new_e+1) */
            divisor_count = divisor_count / (old_e + (idx_t)1)
                                      * (new_e + (idx_t)1);
            exp_arr[idx] = new_e;
        }

        /* store result for this n */
        cache[cur] = divisor_count;
    }

    max_n = n;
    return cache[n];
}
