/*
 * A277165  Lexicographically least sequence of nonnegative integers that
 *          avoids 27/23-powers.
 * https://oeis.org/A277165
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 20000, u16 n <= 20000, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 136 terms through n = 9537 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Lexicographically least sequence over {0,1,2} avoiding 27/23‑powers. */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Check whether appending `cand` to the current prefix `seq[0..len-1]`
   would create a forbidden factor of the form x·y where |x| = 27k
   and |y| = 4k for some k ≥ 1. */
static bool forbids(const seq_t *seq, size_t len, seq_t cand)
{
    size_t new_len = len + 1;
    for (size_t k = 1; k * 27 <= new_len; ++k) {
        size_t block_start  = new_len - 27 * k;
        size_t second_start = block_start + 23 * k;
        size_t compare_len  = 4 * k;
        bool ok = true;
        for (size_t i = 0; i < compare_len; ++i) {
            seq_t v1 = seq[block_start + i];
            size_t pos2 = second_start + i;
            seq_t v2 = (pos2 == len) ? cand : seq[pos2];
            if (v1 != v2) {
                ok = false;
                break;
            }
        }
        if (ok) return true;   /* forbidden pattern found */
    }
    return false;               /* safe to append */
}

seq_t a(idx_t n)
{
    static seq_t *cache     = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* Ensure the cache has space for indices up to n. */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 256;
        while (new_cap <= (size_t)n + 1) new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        cache = tmp;          /* realloc handles NULL as malloc */
        cache_cap = new_cap;
    }

    /* Generate sequence up to the requested index if necessary. */
    while (cache_len <= (size_t)n) {
        for (seq_t cand = 0; cand <= 2; ++cand) {
            if (!forbids(cache, cache_len, cand)) {
                cache[cache_len++] = cand;
                break;
            }
        }
    }

    return cache[(size_t)n];
}
