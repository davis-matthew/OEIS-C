/*
 * A248697  Primes of the form k+(k+3)^2 where k is a nonnegative integer.
 * https://oeis.org/A248697
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = prime of the form k^2 + 7k + 9 where k ≥ 0
 * Range:     u8 n <= 4, u16 n <= 44, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth prime that equals k²+7k+9, offset 1. */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/isPrimeGeneric.h"

seq_t a(idx_t n)
{
    /* lazy static cache of found primes */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_cnt = 0;
    static idx_t last_k = 0;

    if (n == 0) { /* undefined, but return 0 in this case */
        return 0;
    }

    /* Build cache until it contains the nth prime */
    while (cache_cnt < (size_t)n) {
        last_k++;
        seq_t p = (seq_t)last_k * last_k + (seq_t)7 * last_k + 9;
        if (isPrimeGeneric(p)) {
            /* grow the cache if necessary */
            if (cache_cnt == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
                if (cache) {
                    memcpy(new_cache, cache, cache_cnt * sizeof(seq_t));
                    free(cache);
                }
                cache = new_cache;
                cache_cap = new_cap;
            }
            cache[cache_cnt++] = p;
        }
    }
    return cache[n - 1];
}
