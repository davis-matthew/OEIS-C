/*
 * A370842  Numbers k that can be added without carries to their digit
 *          reversal (A004086(k)).
 * https://oeis.org/A370842
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   no_carry_exists
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the n‑th nonnegative integer k such that k + rev(k) has no carries during decimal addition. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/no_carry_exists.h"

/* Return the n‑th element of the sequence, 1‑based. */
seq_t a(idx_t n)
{
    /* Persistent cache of found terms. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static seq_t next_candidate = 0;

    if (cache == NULL) {          /* first call: allocate initial cache */
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }

    while (cache_len < n) {       /* produce terms until we have enough */
        seq_t cand = next_candidate;
        if (no_carry_exists(cand)) {
            if (cache_len == cache_cap) {
                cache_cap = (cache_cap == 0) ? 64 : cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                cache = tmp;
            }
            cache[cache_len++] = cand;
        }
        ++next_candidate;
    }

    return cache[n - 1];
}
