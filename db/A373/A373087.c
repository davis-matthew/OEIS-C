/*
 * A373087  k is a term if k is a square and its odd part is divisible by
 *          exactly two distinct primes.
 * https://oeis.org/A373087
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 98, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   count_distinct_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A373087
   Squares of integers whose odd part is divisible by exactly two distinct primes.
   The function generates the sequence by incrementally testing consecutive
   integers and stores the results in a dynamically growing cache so that
   subsequent calls, however out of order, are answered in O(1) after the
   required values have been generated. */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/count_distinct_primes.h"

static seq_t *cache = NULL;            /* cached sequence values */
static idx_t cache_len = 0;            /* number of values in cache   */
static idx_t cache_cap = 0;            /* allocated capacity           */
static idx_t last_m = 0;               /* last integer that was tested */

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* sequence is defined for n >= 1 */

    while (cache_len < n) {
        ++last_m;
        uint64_t odd = (uint64_t)last_m;
        while ((odd & 1ULL) == 0) odd >>= 1;   /* strip powers of 2 */

        if (odd > 1 && count_distinct_primes(odd) == 2) {
            if (cache_len == cache_cap) {
                cache_cap = cache_cap ? 2 * cache_cap : 8;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = (seq_t)last_m * (seq_t)last_m;
        }
    }
    return cache[n - 1];
}
