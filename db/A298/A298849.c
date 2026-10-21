/*
 * A298849  Numbers n where d*n contains the digit d for all of d =
 *          1,2,3,4,5,6,7,8,9.
 * https://oeis.org/A298849
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 1545, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: returns the nth integer n such that for every d = 1..9,
   the product d*n contains the digit d.  The sequence is built on
   demand and memoised in a static cache. */
#include <stdlib.h>
#include <stdbool.h>

static bool contains_digit(seq_t x, int d) {
    if (x == 0) return false;
    while (x > 0) {
        if ((x % 10) == d) return true;
        x /= 10;
    }
    return false;
}

seq_t a(idx_t n) {
    if (n == 0) return 0; /* offset is 1, undefined for n=0 */

    /* Static cache of computed terms */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    static seq_t next_candidate = 1;

    while (cache_len < n) {
        seq_t cand = next_candidate;

        /* candidate must contain digit 1 */
        if (!contains_digit(cand, 1)) {
            next_candidate++;
            continue;
        }

        bool ok = true;
        for (int d = 2; d <= 9 && ok; ++d) {
            seq_t prod = (seq_t)d * cand;
            if (!contains_digit(prod, d))
                ok = false;
        }

        if (ok) {
            if (cache_len == cache_cap) {
                size_t new_cap = (cache_cap == 0) ? 64 : cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = cand;
        }
        next_candidate++;
    }

    return cache[n - 1];
}
