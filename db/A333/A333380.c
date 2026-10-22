/*
 * A333380  Numbers k such that the k-th composition in standard order is
 *          weakly decreasing and covers an initial interval of positive
 *          integers.
 * https://oeis.org/A333380
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Method: brute‑force enumeration of compositions up to the requested index */
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

/* static cache of sequence values */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;
static idx_t next_k = 0;

/* check whether k yields a weakly decreasing composition that covers
   the initial interval of positive integers */
static bool is_valid_k(idx_t k) {
    if (k == 0) return true;                 /* empty composition */

    idx_t prev_pos = 0;        /* previous set‑bit position, starting from 0 */
    idx_t cur_pos  = 1;        /* current bit position (1‑based) */
    idx_t tmp      = k;

    uint64_t seen_mask = 0;
    idx_t    diff_prev = 0;
    bool     first = true;

    while (tmp) {
        if (tmp & 1) {
            idx_t diff = cur_pos - prev_pos;   /* run length */

            /* weakly decreasing after reversal ↔ non‑decreasing diff */
            if (!first && diff < diff_prev) return false;
            diff_prev = diff;

            if (diff > 63) return false;       /* too large for mask */
            seen_mask |= (uint64_t)1 << (diff - 1);

            prev_pos = cur_pos;
            first    = false;
        }
        cur_pos++;
        tmp >>= 1;
    }

    /* all integers 1..maxdiff must appear */
    if (diff_prev == 0) return true;              /* shouldn't happen */
    if (diff_prev >= 64) return false;            /* safety check */

    uint64_t required = ((uint64_t)1 << diff_prev) - 1;
    return (seen_mask & required) == required;
}

/* OEIS sequence A333380 */
seq_t a(idx_t n) {
    if (n == 0) return 0; /* index 0 is unused */

    while (cache_len < (size_t)n) {
        if (is_valid_k(next_k)) {
            if (cache_len >= cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 64;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)next_k;
        }
        next_k++;
    }
    return cache[n - 1];
}
