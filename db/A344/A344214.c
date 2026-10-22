/*
 * A344214  Numbers k such that repeated iterations of f(m) =
 *          (digsum(f(m-1)))^2 + 1 starting from f(1) = k will eventually
 *          yield 5 before any other single-digit number.
 * https://oeis.org/A344214
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   leads_to_five
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A344214: numbers k such that repeated iterations of
   f(m) = (digit sum of m)^2 + 1 starting from m = k eventually
   reach 5 before any other single‑digit number. */
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/leads_to_five.h"

/* cache of found sequence values */
static seq_t *cache = NULL;
static idx_t cached_len = 0;
static idx_t cached_cap = 0;
static idx_t next_candidate = 1;

/* Main sequence function */
seq_t a(idx_t n) {
    if (n == 0) return 0;              /* undefined, but safe guard */

    /* Extend cache until we have n terms */
    while (cached_len < n) {
        seq_t val = (seq_t)next_candidate;
        if (leads_to_five(val)) {
            if (cached_len == cached_cap) {
                size_t new_cap = cached_cap ? cached_cap * 2 : 64;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) return 0;                 /* allocation failed */
                cache = tmp;
                cached_cap = new_cap;
            }
            cache[cached_len++] = val;
        }
        ++next_candidate;
    }
    return cache[n - 1];               /* 1‑based indexing */
}
