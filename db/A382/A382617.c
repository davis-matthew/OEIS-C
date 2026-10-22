/*
 * A382617  Numbers k such that k = m*(m^2 + 1) where m^2 + 1 is prime.
 * https://oeis.org/A382617
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = m*(m^2 + 1) where m is the n-th integer such that m^2 + 1
 *            is prime. The sequence is 1-indexed (a(1) = 2). The
 *            implementation lazily generates terms on demand and caches them
 *            for subsequent calls
 * Range:     u8 n <= 4, u16 n <= 12, u32 n <= 174, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime.h"

/*  ----- Memoized sequence generator -----------------------------------  */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for n = 0, return 0 */

    /* static state across calls */
    static seq_t  *cache      = NULL;   /* array of computed terms   */
    static size_t  cache_size = 0;      /* allocated size of array   */
    static size_t  cache_len  = 0;      /* number of terms stored    */
    static idx_t   next_m     = 1;      /* next m to test            */

    /* enlarge cache if necessary */
    if (cache_len < (size_t)n) {
        /* Allocate initial capacity if needed */
        if (cache == NULL) {
            cache_size = 16;
            cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        }

        /* Generate missing terms */
        while (cache_len < (size_t)n) {
            /* Find the next m with m^2 + 1 prime */
            while (true) {
                uint64_t sq64 = (uint64_t)next_m * next_m + 1ULL;
                if (is_prime((idx_t)sq64)) {
                    seq_t kv = (seq_t)next_m * (seq_t)sq64;   /* k = m*(m^2+1) */

                    /* Grow cache if necessary */
                    if (cache_len + 1 > cache_size) {
                        size_t new_size = cache_size * 2;
                        if (new_size < cache_len + 1) new_size = cache_len + 1;
                        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
                        if (!tmp) { /* allocation failed – abort gracefully */
                            abort();
                        }
                        cache = tmp;
                        cache_size = new_size;
                    }

                    cache[cache_len++] = kv;
                    next_m++;            /* advance to next candidate m  */
                    break;
                }
                next_m++;                /* try next m */
            }
        }
    }

    return cache[(size_t)n - 1];
}
