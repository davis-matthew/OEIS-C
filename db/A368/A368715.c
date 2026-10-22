/*
 * A368715  Numbers that are not coprime to the maximal exponent in their
 *          prime factorization.
 * https://oeis.org/A368715
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 62, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A368715: Numbers k with gcd(k, A051903(k)) > 1
   (A051903(k) is the largest exponent in the prime factorization of k.)
   The function returns the nth term (a(1)=4). */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd.h"          /* seq_t gcd(seq_t a, seq_t b) */
#include "../utils/factor_u64.h"   /* void factor_u64(uint64_t n,
                                      uint64_t *pr, uint64_t *ex,
                                      size_t *cnt) */

seq_t a(idx_t n) {
    /* static state for memoisation */
    static seq_t *cache = NULL;          /* array of found terms           */
    static size_t cache_len = 0;         /* number of stored terms         */
    static size_t cache_cap = 0;         /* allocated capacity             */
    static idx_t next = 2;               /* next integer to test           */

    if (n == 0) return 0;                /* (offset 1) not used but safe   */

    /* grow cache until we have n terms */
    while (cache_len < n) {
        /* factor current candidate */
        uint64_t primes[64];
        uint64_t exps[64];
        size_t cnt = 0;
        factor_u64((uint64_t)next, primes, exps, &cnt);

        /* find maximal exponent */
        uint64_t max_e = 0;
        for (size_t i = 0; i < cnt; ++i)
            if (exps[i] > max_e) max_e = exps[i];

        /* test the criterion */
        if (max_e > 1) {
            seq_t g = gcd((seq_t)next, (seq_t)max_e);
            if (g > 1) {
                /* ensure capacity */
                if (cache_len == cache_cap) {
                    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                    seq_t *new_cache = (seq_t *)realloc(cache,
                                                          new_cap * sizeof(seq_t));
                    if (!new_cache) abort();    /* allocation failed */
                    cache = new_cache;
                    cache_cap = new_cap;
                }
                cache[cache_len++] = (seq_t)next;
            }
        }

        ++next;  /* move to the next integer */
    }

    return cache[n - 1];   /* indexing starts at 1 */
}
