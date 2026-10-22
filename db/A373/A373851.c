/*
 * A373851  a(n) = 1 if A276085(n) is a multiple of bigomega(n), otherwise 0,
 *          where bigomega (A001222) and A276085 are fully additive with a(p)
 *          = 1 and a(p) = p#/p respectively.
 * https://oeis.org/A373851
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if A276085(n) is a multiple of bigomega(n), else 0
 * Range:     u8 n <= 9899, u16 n <= 9899, u32 n <= 9899, u64 n <= 9899,
 *            u128 n <= 9899, big any n
 * Helpers:   bigomega, build_primes_upto, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/bigomega.h"
#include "../utils/factor_u64.h"
#include "../utils/build_primes_upto.h"

seq_t a(idx_t n)
{
    /* Handle the offset n = 1 explicitly */
    if (n == 1)
        return (seq_t)1;

    /* Get Ω(n) – the total number of prime factors (counted with multiplicity) */
    seq_t mod = bigomega(n);
    if (mod == 0 || mod == 1)
        return (seq_t)1;      /* for n>1, Ω(n)=1 (prime) or Ω(n)=0 only when n=1 */

    /* Factorize n */
    uint64_t primes[64], exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* Find the largest prime factor in order to build a prime list up to p-1 */
    uint64_t max_p = 0;
    for (size_t i = 0; i < cnt; ++i)
        if (primes[i] > max_p)
            max_p = primes[i];

    /* Static cache for the list of primes up to max_p-1 */
    static uint64_t *prime_cache = NULL;
    static size_t prime_cache_cnt = 0;
    static uint64_t prime_cache_maxlimit = 0;

    if (max_p <= prime_cache_maxlimit) {            /* reuse existing cache */
    } else {                                        /* rebuild cache */
        if (prime_cache)
            free(prime_cache);
        uint64_t limit = max_p > 1 ? max_p - 1 : 0;
        build_primes_upto(limit, &prime_cache, &prime_cache_cnt);
        prime_cache_maxlimit = max_p - 1;
    }

    /* Compute A276085(n) modulo Ω(n) */
    seq_t sum_mod = 0;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];

        /* product of all primes < p modulo mod */
        seq_t prod = 1 % mod;
        for (size_t j = 0; j < prime_cache_cnt && prime_cache[j] < p; ++j) {
            uint64_t prime_mod64 = prime_cache[j] % (uint64_t)mod;
            prod = (prod * (seq_t)prime_mod64) % mod;
        }

        seq_t exp_mod = (seq_t)(e % (uint64_t)mod);
        seq_t term = (prod * exp_mod) % mod;
        sum_mod = (sum_mod + term) % mod;
    }

    return (sum_mod == 0) ? (seq_t)1 : (seq_t)0;
}
