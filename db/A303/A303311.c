/*
 * A303311  Number of nX5 0..1 arrays with every element equal to 0, 1, 4, 5
 *          or 6 horizontally, diagonally or antidiagonally adjacent elements,
 *          with upper left element zero.
 * https://oeis.org/A303311
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 23, u64 n <= 47, u128 n <= 95,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A303311
   Recurrence: a(n) = a(n-1) + 9*a(n-3) + 2*a(n-4) + 4*a(n-5) - 10*a(n-6) - 6*a(n-7) + 4*a(n-9)
   (n > 12)
   Initial values (n = 1 … 12):
   8, 7, 24, 50, 139, 349, 853, 2180, 5525, 13824, 34900, 88161
   Offset: 1 */

#include <stdlib.h>

static const seq_t init[12] = {
    (seq_t)8,
    (seq_t)7,
    (seq_t)24,
    (seq_t)50,
    (seq_t)139,
    (seq_t)349,
    (seq_t)853,
    (seq_t)2180,
    (seq_t)5525,
    (seq_t)13824,
    (seq_t)34900,
    (seq_t)88161
};

seq_t a(idx_t n) {
    /* Memoized computation with dynamic array. */
    static seq_t *cache = NULL;
    static size_t cached_len = 0;          /* greatest n already computed */

    if (n <= cached_len) {
        return cache[n];
    }

    size_t old_len = cached_len;
    size_t new_len = (size_t)n;
    cache = (seq_t *)realloc(cache, sizeof(seq_t) * (new_len + 1));
    cached_len = new_len;

    /* Fill base values up to 12 if not already present. */
    if (old_len <= 12) {
        size_t up_to = new_len < 12 ? new_len : 12;
        for (size_t i = old_len + 1; i <= up_to; ++i) {
            cache[i] = init[i - 1];
        }
    }

    /* Compute values for indices > 12. */
    size_t start = old_len < 13 ? 13 : old_len + 1;
    for (size_t i = start; i <= new_len; ++i) {
        cache[i] = cache[i - 1]
                 + (seq_t)9 * cache[i - 3]
                 + (seq_t)2 * cache[i - 4]
                 + (seq_t)4 * cache[i - 5]
                 - (seq_t)10 * cache[i - 6]
                 - (seq_t)6 * cache[i - 7]
                 + (seq_t)4 * cache[i - 9];
    }

    return cache[n];
}
