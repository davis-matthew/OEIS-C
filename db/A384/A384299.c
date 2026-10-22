/*
 * A384299  Primes p such that p + 8, p + 12 and p + 20 are also primes.
 * https://oeis.org/A384299
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   is_prime_big, seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: primes p such that p+8, p+12, p+20 are all prime. */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;              /* Offset 1 */

    static seq_t *cache   = NULL;             /* Cached qualified primes */
    static size_t cache_size = 0;             /* Number of cached terms   */
    static size_t cache_cap  = 0;             /* Capacity of the cache    */
    static uint64_t last_prime = 0;           /* Last prime examined.     */

    const size_t need = (size_t)n;

    /* Expand the cache until we have at least `need` terms */
    while (cache_size < need) {
        /* Find the next prime after `last_prime` */
        uint64_t cand = last_prime + 1;
        if (cand < 2) cand = 2;
        else if ((cand & 1ULL) == 0) cand++;   /* Ensure odd candidate */

        while (!is_prime_big((seq_t)cand)) {
            if ((cand & 1ULL) == 0) cand += 1;
            else cand += 2;
        }
        last_prime = cand;                     /* cand is now prime        */

        /* Test the required pattern */
        seq_t p   = (seq_t)cand;
        seq_t p8  = p + (seq_t)8;
        seq_t p12 = p + (seq_t)12;
        seq_t p20 = p + (seq_t)20;

        if (is_prime_big(p8) && is_prime_big(p12) && is_prime_big(p20)) {
            /* Ensure capacity for the new term */
            if (cache == NULL) {
                cache_cap = 4;
                cache = seq_alloc(cache_cap);
            } else if (cache_size >= cache_cap) {
                size_t new_cap = cache_cap * 2;
                seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
                if (tmp == NULL) {                /* allocation failed */
                    free(cache);
                    cache = NULL;
                    return (seq_t)0;
                }
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_size++] = p;
        }
    }

    return cache[need - 1];
}
