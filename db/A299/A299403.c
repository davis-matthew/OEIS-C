/*
 * A299403  Lexicographic first sequence of positive integers such that
 *          a(n)*a(n+1) has a digit 3, and no term occurs twice.
 * https://oeis.org/A299403
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(1)=1; for n>1, a(n) = smallest unused positive integer m such
 *            that a(n‑1)·m contains the decimal digit 3. Each term is unique
 * Range:     u8 n <= 6, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   contains_digit3, grow_used_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/contains_digit3.h"
#include "../utils/grow_used_array.h"

/* Grow a dynamic array of seq_t if capacity is too small. */
static void grow_seq_array(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap >= need) return;
    size_t old_cap = *cap;
    size_t new_cap = (*cap == 0) ? 8 : (*cap << 1);
    while (new_cap < need) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    *arr = tmp;
    *cap = new_cap;
    (void)old_cap; /* unused when old_cap==0 */
}

/* ------------------------------------------------------------------ */
/* Public function required by the build system.                       */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0; /* undefined for n < 1 */

    /* Static cache of computed terms and bookkeeping. */
    static seq_t *cache   = NULL;
    static size_t  cache_cap   = 0;
    static size_t  cache_len   = 0;

    static bool *used   = NULL;
    static size_t used_cap = 0;

    /* Ensure storage is allocated. */
    if (!cache) grow_seq_array(&cache, &cache_cap, 16);
    if (!used)  grow_used_array(&used, &used_cap, 16);

    /* Generate terms up to the requested index. */
    while (cache_len < (size_t)n) {
        seq_t next_val = 0;
        if (cache_len == 0) {
            /* a(1) = 1 by definition. */
            next_val = (seq_t)1;
            /* Ensure used[1] exists and mark it. */
            if (1 >= used_cap) grow_used_array(&used, &used_cap, 16);
            used[1] = true;
        } else {
            seq_t prev = cache[cache_len - 1];

            /* Search for the smallest unused integer whose product with prev
               contains the digit 3. */
            for (idx_t cand = 1; ; ++cand) {
                size_t idx = (size_t)cand;          /* array index is 0‑based */
                if (idx >= used_cap) grow_used_array(&used, &used_cap, idx + 8);
                if (used[idx]) continue;           /* already used */

                seq_t prod = prev * (seq_t)cand;
                if (contains_digit3(prod)) {
                    next_val = (seq_t)cand;
                    used[idx] = true;
                    break;
                }
            }
        }

        /* Store the newly found term. */
        grow_seq_array(&cache, &cache_cap, cache_len + 1);
        cache[cache_len++] = next_val;
    }

    return cache[(size_t)n - 1];
}
