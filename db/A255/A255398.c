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
 * Validated: standard, pass, 82 terms through n = 9517 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/mem.h"

/* helper: test whether a sequence integer has no digit 1 */
static inline bool no_digit_one(seq_t x)
{
    if (x == 0) return true;        /* 0 contains no '1' */
    while (x > 0) {
        if ((x % 10) == 1) return false;
        x /= 10;
    }
    return true;
}

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
            cache = mem_new(seq_t, cache_cap);
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
                cache = mem_resize(seq_t, cache, new_cap);
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)k;
        }
        last_checked = k;
    }

    return cache[n - 1];
}
