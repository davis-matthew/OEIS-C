/*
 * A392480  Squares with non-distinct digits.
 * https://oeis.org/A392480
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 150, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has_repeated_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A392480: Squares with non‑distinct decimal digits.
   The function searches for squares k^2 whose decimal representation
   contains at least one repeated digit and memoizes the results.
   It returns the n‑th such square for any n ≥ 1 (offset 1). */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has_repeated_digits.h"

/* Memoization buffer for computed terms. */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;
static idx_t next_k = 1;          /* next integer to test as a square root */

/* Return the n‑th term of A392480, where n >= 1. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;         /* behavior for n < 1 is unspecified */

    /* Generate terms until we have at least n of them. */
    while (cache_len < n) {
        seq_t sq = (seq_t)next_k * (seq_t)next_k;   /* k^2 */

        if (has_repeated_digits(sq)) {
            /* Grow buffer if necessary. */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();   /* allocation failure */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = sq;
        }
        ++next_k;
    }

    return cache[n - 1];
}
