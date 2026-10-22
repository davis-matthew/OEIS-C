/*
 * A366761  Numbers that have at least one exponent in their canonical prime
 *          factorization that is divisible by 3.
 * https://oeis.org/A366761
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   numbers that have at least one exponent in their canonical prime
 *            factorization that is divisible by 3. Computed by trial division
 * Range:     u8 n <= 25, u16 n <= 6868, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has_cube_exponent
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/has_cube_exponent.h"

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache     = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static seq_t next_candidate = 1;   /* next integer to test */

    /* Generate terms until we have at least n terms cached */
    while (cache_len < (size_t)n) {
        if (has_cube_exponent(next_candidate)) {
            if (cache_len == cache_cap) {
                size_t new_cap = (cache_cap ? cache_cap * 2 : 16);
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp)   /* allocation failure – unlikely for the limits used */
                    return 0;
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = next_candidate;
        }
        ++next_candidate;
    }
    return cache[n - 1];
}
