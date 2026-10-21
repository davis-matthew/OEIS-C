/*
 * A247761  Numbers in decimal representation, such that in Russian their
 *          digits are in alphabetic order.
 * https://oeis.org/A247761
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, word
 * Formula:   not stated
 * Range:     u8 n <= 109, u16 n <= 1592, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence a(n) is the n-th nonnegative integer whose decimal digits
   appear in non‑decreasing order according to the Russian
   digit‑name alphabet: 8, 2, 9, 0, 1, 5, 7, 3, 4, 6. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static inline bool _is_valid(idx_t x)
{
    /* order[d] = position of digit d in the Russian lexicographic order.
       Smaller value means earlier in the alphabet. */
    static const uint8_t order[10] = {
        3, /* 0 */
        4, /* 1 */
        1, /* 2 */
        7, /* 3 */
        8, /* 4 */
        5, /* 5 */
        9, /* 6 */
        6, /* 7 */
        0, /* 8 */
        2  /* 9 */
    };

    if (x == 0)
        return true;            /* zero is always valid */

    int digits[20];
    int len = 0;
    while (x) {
        digits[len++] = (int)(x % 10);   /* least significant first */
        x /= 10;
    }

    /* digits[0] = least significant, digits[len-1] = most significant.
       The digits must be in non‑decreasing order from most to least
       significant.  For each pair of consecutive digits, the more
       significant one must not have a higher order value. */
    for (int i = 1; i < len; ++i) {
        if (order[digits[i]] > order[digits[i - 1]])
            return false;           /* decreasing order found */
    }
    return true;
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static idx_t next_candidate = 0;

    if (n == 0)
        return (seq_t)0;            /* sequence defined for n ≥ 1 */

    size_t target = (size_t)n;
    while (cache_len < target) {
        if (_is_valid(next_candidate)) {
            if (cache_len == cache_cap) {
                size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) {           /* allocation failure: abort */
                    exit(1);
                }
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len] = (seq_t)next_candidate;
            ++cache_len;
        }
        ++next_candidate;
    }
    return cache[target - 1];
}
