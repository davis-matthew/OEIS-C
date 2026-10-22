/*
 * A347746  Positive integers that are equal both to the product of two
 *          integers ending with 4 and to that of two integers ending with 6.
 * https://oeis.org/A347746
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   numbers that can be expressed as the product of two integers
 *            ending with 4 and also as the product of two integers ending
 *            with 6 (OEIS A347746). The implementation searches sequentially
 *            for successive integers ending in 6 and tests whether they admit
 *            factor pairs \((x\,\bmod10=4)\) and \((y\,\bmo...
 * Range:     u8 none, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"

/* Utility: does n admit a factor pair whose both members end with 'last'? */
static bool has_pair_with_last_digits(seq_t n, int last)
{
    seq_t sq = floor_sqrt(n);
    idx_t limit = (idx_t)sq;                 /* sqrt(n) always fits in idx_t for our use case */
    for (idx_t a = 1; a <= limit; ++a) {
        if (n % (seq_t)a == 0) {
            seq_t b = n / (seq_t)a;
            if ((a % 10) == last && (b % 10) == last)
                return true;
        }
    }
    return false;
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;          /* cached terms */
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static seq_t cur = 6;                /* last candidate tested (always ends with 6) */

    /* grow cache as needed */
    while (cache_len < n) {
        cur += 10;                       /* next integer ending in 6 */

        if (has_pair_with_last_digits(cur, 4) &&
            has_pair_with_last_digits(cur, 6)) {
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap == 0 ? 16 : cache_cap * 2;
                cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                cache_cap = new_cap;
            }
            cache[cache_len++] = cur;
        }
    }

    return cache[n - 1];
}
