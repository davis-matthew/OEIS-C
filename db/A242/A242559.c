/*
 * A242559  Zeroless numbers n such that n^3 contains a 0.
 * https://oeis.org/A242559
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 4, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A242559: Zeroless numbers n such that n^3 contains a decimal 0
   a(n) is the n-th such number (offset 1).  This implementation
   searches successive integers, keeps those with no zero digit whose
   cubes contain at least one zero digit, and memoises the results
   so that any a(n) can be returned in O(1) time after up to the
   requested n integers have been examined. */
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/seq_realloc.h"      /* for seq_realloc */

/* ---------- helper: check for zero digit in a seq_t value ---------- */
static inline bool has_zero_digit(seq_t x)
{
    if (x == 0) return true;           /* digit '0' present */
    while (x != 0) {
        if (x % 10 == 0) return true;
        x /= 10;
    }
    return false;
}

/* ---------- helper: test if cube of a seq_t contains a zero digit ---------- */
static inline bool cube_has_zero(seq_t n)
{
    seq_t cube = n * n * n;           /* n³ */
    return has_zero_digit(cube);
}

/* ---------- static cache of found terms ---------- */
static seq_t *cache = NULL;          /* array of terms */
static size_t cache_cap = 0;         /* allocated capacity */
static size_t cache_len = 0;         /* number of terms stored */

/* next integer to examine (starts at 1) */
static seq_t next_candidate = 1;

/* ---------- ensure cache holds at least 'need' elements ---------- */
static void ensure_cache_size(size_t need)
{
    if (cache_len >= need) return;

    /* grow the array if required */
    if (cache_cap < need) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 128;
        while (new_cap < need) new_cap <<= 1;
        /* seq_realloc returns a new pointer or the old one */
        seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
        cache = tmp;
        cache_cap = new_cap;
    }

    /* extend the cache by searching further candidates */
    while (cache_len < need) {
        seq_t cand = next_candidate;
        if (!has_zero_digit(cand) && cube_has_zero(cand)) {
            cache[cache_len++] = cand;
        }
        ++next_candidate;
    }
}

/* ---------- public routine a(idx_t n) ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* offset is 1 */
    ensure_cache_size((size_t)n);
    return cache[n - 1];
}
