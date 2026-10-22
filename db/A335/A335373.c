/*
 * A335373  Numbers k such that the k-th composition in standard order
 *          (A066099) is not unimodal.
 * https://oeis.org/A335373
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 56, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   is_unimodal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A335373: 
 * a(n) = the n‑th integer k such that the k‑th composition in standard
 * order (A066099) is not unimodal.  The function generates compositions
 * until it has found n non‑unimodal ones. */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/is_unimodal.h"

/* Static storage for memoisation of the sequence values. */
static seq_t *cache = NULL;
static size_t cache_len = 0;   /* number of elements stored in cache      */
static size_t cache_cap = 0;   /* capacity of the cache array              */
static idx_t next_k = 1;       /* next composition index to examine      */

/* Ensure that cache contains at least n terms.
 * The function keeps generating compositions until it has found
 * n non‑unimodal compositions. */
static void ensure_cache(idx_t n)
{
    if (n == (idx_t)0)
        return;

    /* Grow the cache array as needed. */
    if (n > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 8;
        if (new_cap < (size_t)n)
            new_cap = (size_t)n;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Generate compositions until we have enough non‑unimodal ones. */
    while (cache_len < (size_t)n) {
        idx_t k = next_k;

        /* Build the ordered list of positions of 1 bits in the
         * reversed binary representation of k.  Bits are examined
         * from least significant upward, so positions start at 1. */
        int pos[65];
        int pos_cnt = 1;      /* pos[0] = 0 will be added below. */
        pos[0] = 0;
        idx_t t = k;
        int bit_pos = 1;      /* position (1‑based) of current bit */
        while (t) {
            if (t & 1ULL)
                pos[pos_cnt++] = bit_pos;
            t >>= 1;
            ++bit_pos;
        }

        /* Compute the differences between consecutive positions
         * to obtain the composition parts. */
        int parts[64];
        int parts_cnt = pos_cnt - 1;
        for (int i = 0; i < parts_cnt; ++i)
            parts[i] = pos[i + 1] - pos[i];

        /* Reverse the parts to get the final composition. */
        int rev[64];
        for (int i = 0; i < parts_cnt; ++i)
            rev[i] = parts[parts_cnt - 1 - i];

        /* If the composition is not unimodal, store its index k. */
        if (!is_unimodal(rev, parts_cnt)) {
            cache[cache_len++] = (seq_t)k;
        }

        ++next_k;
    }
}

/* Return the n-th term (1-indexed) of the sequence. */
seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)0;   /* undefined for offset 0, return 0 */

    ensure_cache(n);
    return cache[(size_t)(n - 1)];
}
