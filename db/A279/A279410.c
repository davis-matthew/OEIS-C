/*
 * A279410  Numbers whose squares have identical middle digits in base 10.
 * https://oeis.org/A279410
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th positive integer whose square has an even number
 *            of digits and whose two middle digits are equal. Computed by a
 *            simple incremental search
 * Range:     u8 none, u16 n <= 7, u32 n <= 4352, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_len, pow10, seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/pow10.h"
#include "../utils/digit_len.h"
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

static bool has_identical_middle_digits(seq_t sq) {
    int len = digit_len(sq);
    if (len & 1) return false;          /* odds are ignored */
    unsigned int k = (len / 2) - 1;     /* exponent of 10^k that shifts the middle two digits out */
    seq_t powk = pow10((idx_t)k);       /* 10^k as seq_t */
    seq_t middle = (sq / powk) % (seq_t)100; /* the two middle digits */
    return ((middle / 10) % 10) == (middle % 10);
}

seq_t a(idx_t n) {
    /* Lazy cache of already found terms */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    static seq_t last_checked = 0;

    if (n == 0) return 0; /* undefined for 0 but harmless */

    while (cache_len < n) {
        seq_t cand = last_checked + 1;
        seq_t square = cand * cand;
        if (has_identical_middle_digits(square)) {
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                cache = seq_realloc(cache, cache_cap, new_cap);
                cache_cap = new_cap;
            }
            cache[cache_len++] = cand;
        }
        last_checked = cand;
    }
    return cache[n - 1];
}
