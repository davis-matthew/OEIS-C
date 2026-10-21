/*
 * A255398  Numbers k such that k^2 lacks the digit 1 in its decimal
 *          expansion.
 * https://oeis.org/A255398
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   numbers k such that k^2 has no digit 1
 * Range:     u8 n <= 11, u16 n <= 1175, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   no_digit_one
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/no_digit_one.h"

seq_t a(idx_t n)
{
    /* static cache of found terms */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    static idx_t last_checked = 0;

    /* initialize cache with the first term 0 */
    if (cache_len == 0) {
        if (cache_cap == 0) {
            cache_cap = 1;
            cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        }
        cache[0] = (seq_t)0;
        cache_len = 1;
        last_checked = 0;
    }

    /* generate terms until we have at least n of them */
    while (cache_len < n) {
        idx_t k = last_checked + 1;
        seq_t sq = ((seq_t)k) * ((seq_t)k);
        if (no_digit_one(sq)) {
            /* ensure capacity for new element */
            if (cache_len + 1 > cache_cap) {
                size_t new_cap = cache_cap * 2;
                if (new_cap == 0) new_cap = 1;
                cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)k;
        }
        last_checked = k;
    }

    return cache[n - 1];
}
