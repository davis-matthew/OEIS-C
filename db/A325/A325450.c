/*
 * A325450  Numbers k such that sum of digits (k) and sum of digits (k^2) is
 *          9.
 * https://oeis.org/A325450
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 15, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   digit_sum, seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A325450: numbers k such that digit_sum(k) = 9 and digit_sum(k^2) = 9.
   The function generates the sequence by testing successive multiples of 9. */

/* Requires util headers for seq_t allocation and digit_sum. */
#include <stdbool.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"
#include "../utils/digit_sum.h"

seq_t a(idx_t n)
{
    /* Static cache of found terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static seq_t next_candidate = (seq_t)9;  /* first potential k */

    /* Generate terms until we have at least n */
    while (cache_len < n) {
        /* Condition: both digit sums equal 9 */
        if ((digit_sum(next_candidate) == (seq_t)9) &&
            (digit_sum(next_candidate * next_candidate) == (seq_t)9)) {
            /* Ensure capacity */
            if (cache_len == cache_cap) {
                cache_cap = cache_cap ? cache_cap * 2 : 16;
                cache = seq_realloc(cache, cache_len, cache_cap);
            }
            cache[cache_len++] = next_candidate;
        }
        /* Next multiple of 9 */
        next_candidate += (seq_t)9;
    }

    return cache[n - 1];
}
