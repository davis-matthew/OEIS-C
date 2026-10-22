/*
 * A354081  Positive integers k such that the first digit of k is divisible by
 *          the product of all the remaining digits of k.
 * https://oeis.org/A354081
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the nth positive integer k such that the first * decimal
 *            digit of k is divisible by the product of the remaining digits.
 *            * The function computes terms by incremental search and
 *            memoisation
 * Range:     u8 n <= 36, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   is_term
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_term.h"

seq_t a(idx_t n)
{
    static seq_t *cache   = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static seq_t next = 1;    /* next candidate number */

    if (n == 0)
        return 0;            /* undefined for n < 1 */

    /* Grow the cache until it contains at least n terms. */
    while (cache_len < (size_t)n) {
        if (is_term(next)) {
            if (cache_len == cache_cap) {
                size_t new_cap = (cache_cap == 0) ? 64 : cache_cap << 1;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();   /* allocation failed */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = next;
        }
        next++;   /* advance to the next candidate */
    }
    return cache[(size_t)n - 1];
}
