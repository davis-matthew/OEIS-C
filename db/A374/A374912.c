/*
 * A374912  Primes p such that (p - 1)^p == p (mod 2*p - 1).
 * https://oeis.org/A374912
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 440, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes p such that (p-1)^p == p (mod 2p-1)
 * The function implements a direct search; no closed‑form formula.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    /* static cache of found primes */
    static seq_t *cache = NULL;
    static size_t cache_len = 0, cache_cap = 0;
    static uint64_t last_checked = 0;   /* last candidate examined (odd integer) */

    /* a(1) = 3 explicitly */
    if (n == 0) return (seq_t)0;        /* undefined, safety for n=0 */

    /* one‑time initialisation */
    if (cache_len == 0) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0] = (seq_t)3;
        cache_len = 1;
        last_checked = 3;
    }

    /* extend cache until we have n terms */
    while (cache_len < (size_t)n) {
        /* check next odd integer as a primality candidate */
        last_checked += 2;
        if (!is_prime((idx_t)last_checked))
            continue;                     /* skip if not prime */

        seq_t base = (seq_t)(last_checked - 1);
        seq_t mod  = (seq_t)(2ULL * (uint64_t)last_checked - 1ULL);

        /* compute (p-1)^p mod (2p-1) */
        seq_t val = mod_pow(base, (uint64_t)last_checked, mod);

        if (val == (seq_t)last_checked) { /* condition satisfied */
            if (cache_len == cache_cap) {  /* grow cache dynamically */
                cache_cap *= 2;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = (seq_t)last_checked;
        }
    }

    return cache[n - 1];
}
