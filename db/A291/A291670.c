/*
 * A291670  Numbers k such that 2 is the smallest decimal digit of k^4.
 * https://oeis.org/A291670
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 9, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   smallest_digit_is_two
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A291670: numbers k such that 2 is the smallest decimal digit of k^4 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/smallest_digit_is_two.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* undefined for n < 1, harmless */

    /* Static cache of found terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;          /* number of terms stored */
    static size_t cache_cap = 0;          /* allocated capacity */
    static idx_t last_k = 0;              /* last candidate checked */

    while (cache_len < (size_t)n) {
        last_k++;                          /* next candidate */

        seq_t k2 = (seq_t)last_k * (seq_t)last_k; /* k^2 */
        seq_t k4 = k2 * k2;                /* k^4 */

        if (smallest_digit_is_two(k4)) {
            /* Append term to cache */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();         /* allocation failure */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)last_k;
        }
    }

    return cache[(size_t)n - 1];
}
