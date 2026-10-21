/*
 * A268585  Composite numbers m such that for any positive integers a < b, if
 *          a * b = m then b - a is a perfect square.
 * https://oeis.org/A268585
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3, u32 n <= 29, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   build_primes_upto, floor_sqrt, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A268585: composite numbers m such that for any a < b with a*b = m,
   the difference b - a is a perfect square.  Implementation finds such
   numbers in ascending order by testing successive candidates m = t^2 + 1
   and checking that m factors into exactly two distinct primes p<q with
   q-p a square.  The sequence values are cached so any call order is
   supported. */

/* include portable utilities from the framework */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* prime list builder and primality tests */
#include "../utils/build_primes_upto.h" /* build_primes_upto(idx_t, uint64_t **, size_t *) */
#include "../utils/is_prime.h"           /* is_prime(idx_t) */

/* integer square root in the seq_t type */
#include "../utils/floor_sqrt.h"         /* floor_sqrt(seq_t) */

seq_t a(idx_t n)
{
    /* static state for caching */
    static seq_t *cache      = NULL;   /* cached sequence terms          */
    static size_t cache_sz   = 0;      /* number of cached terms         */
    static size_t cache_cap  = 0;      /* allocated capacity of cache    */
    static uint64_t *primes  = NULL;   /* list of primes                 */
    static size_t primes_cnt = 0;      /* number of primes               */
    static uint64_t t_current = 1;     /* next t to test                 */

    /* Lazily build a modest prime sieve – enough for the first few
       thousands of terms.  This limit is sufficient for n up to 10 000
       (values < 5 e7); it can be increased easily if needed. */
    if (primes == NULL) {
        const idx_t LIMIT = 20000000u;
        build_primes_upto(LIMIT, &primes, &primes_cnt);
    }

    /* Generate terms until the cache contains at least n entries. */
    while (cache_sz < n) {
        /* candidate value: t^2 + 1 */
        __uint128_t tt = (__uint128_t)t_current * t_current + 1;
        seq_t m = (seq_t)tt;

        /* attempt to factor m into two distinct primes p<q */
        uint64_t temp = (uint64_t)m;          /* we assume m fits in 64 bits for the required n */
        uint64_t p = 0, q = 0;
        bool two_factors = false;

        if (temp > 1) {
            for (size_t i = 0; i < primes_cnt; ++i) {
                uint64_t prime = primes[i];
                if (prime * prime > temp)
                    break;
                if (temp % prime == 0) {
                    p = prime;
                    temp /= prime;
                    /* reject if p divides again (p^2 factor) */
                    if (temp % prime == 0)
                        break;
                    /* remaining factor must be prime */
                    if (is_prime((idx_t)temp)) {
                        q = temp;
                        if (p < q)
                            two_factors = true;
                    }
                    break;  /* only the first factor matters */
                }
            }
        }

        if (two_factors) {
            /* check that q - p is a perfect square */
            uint64_t diff = q - p;
            seq_t diff_seq = (seq_t)diff;
            seq_t s_seq = floor_sqrt(diff_seq);
            uint64_t s = (uint64_t)s_seq;
            if ((seq_t)s * (seq_t)s == diff_seq) {
                /* accept this number */
                if (cache_sz == cache_cap) {
                    size_t new_cap = cache_cap ? cache_cap << 1 : 16;
                    seq_t *new_mem = (seq_t *)realloc(cache,
                                                       new_cap * sizeof(seq_t));
                    if (!new_mem)
                        abort();           /* allocation failed */
                    cache      = new_mem;
                    cache_cap  = new_cap;
                }
                cache[cache_sz++] = m;
            }
        }
        ++t_current;
    }

    return cache[n - 1];
}
