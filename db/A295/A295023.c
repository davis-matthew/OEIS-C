/*
 * A295023  Cubes whose largest digit is 8.
 * https://oeis.org/A295023
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A294998(n)^3 (cubes whose largest decimal digit is 8)
 * Range:     u8 n <= 1, u16 n <= 9, u32 n <= 450, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   cube_largest_digit_is8
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/cube_largest_digit_is8.h"

seq_t a(idx_t n)
{
    /* static cache of already found cubes */
    static seq_t *cache = NULL;   /* array of cubes */
    static size_t cache_len = 0;  /* number of elements in cache */
    static size_t cache_cap = 0;  /* allocated capacity */
    static idx_t last_checked_m = 0;  /* last m that was examined */

    /* if cache is empty, allocate a small initial block */
    if (cache_len == 0) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }

    /* extend cache until we have the requested n-th term */
    while (cache_len < (size_t)n) {
        /* generate next m */
        idx_t m = last_checked_m + 1;
        last_checked_m = m;

        /* compute m^3 */
        seq_t ms = (seq_t)m;
        seq_t cube = ms * ms * ms;

        /* if this cube satisfies the rule, store it */
        if (cube_largest_digit_is8(cube)) {
            if (cache_len == cache_cap) {
                /* grow cache, double capacity */
                size_t new_cap = cache_cap * 2;
                seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!new_cache) { /* allocation failure; abort gracefully */
                    abort();
                }
                cache = new_cache;
                cache_cap = new_cap;
            }
            cache[cache_len++] = cube;
        }
    }

    /* cache is zero-indexed, a(1) is first term stored */
    return cache[(size_t)(n - 1)];
}
