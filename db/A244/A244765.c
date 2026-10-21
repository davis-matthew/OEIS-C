/*
 * A244765  Prime numbers ending in the prime number 19.
 * https://oeis.org/A244765
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is the nth prime that ends with 19 (primes of the form
 *            100*k + 19)
 * Range:     u8 n <= 1, u16 n <= 165, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/isPrimeGeneric.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* static cache of primes already found */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* allocated size of cache */
    static size_t cache_len = 0;   /* number of primes actually stored */

    /* index of the next candidate of the form 100*k + 19 */
    static idx_t next_k = 0;

    /* generate primes until we have at least n of them */
    while (cache_len < n) {
        seq_t cand = 100 * (seq_t)next_k + 19;   /* candidate 100*k+19 */

        if (isPrimeGeneric(cand)) {
            /* grow cache if necessary */
            if (cache_len == cache_cap) {
                size_t new_cap = (cache_cap == 0 ? 8 : cache_cap * 2);
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) { /* allocation failed */
                    free(cache);
                    cache = NULL;
                    cache_cap = 0;
                    cache_len = 0;
                    return 0;
                }
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = cand;
        }
        ++next_k;   /* move to the next candidate 100*k+19 */
    }

    return cache[n - 1];
}
