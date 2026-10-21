/*
 * A282693  Numbers k such that k = (sum of digits of k)*((sum of digits of k)
 *          + 1).
 * https://oeis.org/A282693
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  bref, base, fini, full, nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 5, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Brute‑force search for A282693: a(n) is the nth integer k that satisfies
 * k = digit_sum(k) * (digit_sum(k) + 1).  The function builds a cache of
 * all such k as it is called and returns the requested element. */
#include <stddef.h>
#include <stdlib.h>
#include "../utils/digit_sum.h"

seq_t a(idx_t n)
{
    /* static cache of found terms */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap  = 0;
    static idx_t  current_s  = 0;   /* next digit–sum to test */

    /* Grow cache until it contains n elements */
    while (cache_size < (size_t)n) {
        seq_t s = (seq_t)current_s;
        seq_t k = s * (s + 1);

        /* Detect overflow of seq_t multiplication; once overflow occurs
         * all larger s would overflow as well, so we stop the search. */
        if (s != 0 && k / s != s + 1) {
            break;
        }

        if (digit_sum(k) == s) {
            /* enlarge cache if necessary */
            if (cache_size == cache_cap) {
                size_t new_cap = (cache_cap == 0) ? 8 : cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) {          /* allocation failed – abort */
                    return 0;
                }
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_size++] = k;
        }
        ++current_s;
    }

    /* Return the requested element; if n exceeds the sequence length,
     * 0 is returned (undefined). */
    if (n == 0 || n > cache_size) {
        return 0;
    }
    return cache[n - 1];
}
