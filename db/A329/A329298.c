/*
 * A329298  Numbers whose digits are in nondecreasing order in bases 8 and 9.
 * https://oeis.org/A329298
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 34, u16 n <= 52, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   nondecreasing
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: iterate over integers, collecting those whose base‑8 and base‑9
 * digits are in nondecreasing order. A dynamic cache allows arbitrary call order. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/nondecreasing.h"

static bool is_valid(seq_t v)
{
    return nondecreasing(v, 8) && nondecreasing(v, 9);
}

/* ---------- cache storage ---------- */
static seq_t   *cache          = NULL;
static size_t   cache_len      = 0;
static size_t   cache_cap      = 0;
static idx_t    next_candidate = 0;          /* next integer to test */

/* maximum value that can be represented by seq_t if it is a fixed unsigned
 * type of size ≤8 bytes; otherwise we use the sentinel (idx_t)-1 to mean
 * unlimited. */
static const idx_t MAX_SEQ_VAL =
    (sizeof(seq_t) <= 8 ? (idx_t)((seq_t)-1) : (idx_t)-1);

static void grow_cache(size_t needed)
{
    if (cache_cap >= needed) return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 16;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) exit(1);
    cache = new_cache;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* undefined but safe guard */
    while (cache_len < (size_t)n) {
        if (next_candidate > MAX_SEQ_VAL) break;   /* no more representable */
        seq_t cand = (seq_t)next_candidate;
        if (is_valid(cand)) {
            grow_cache(cache_len + 1);
            cache[cache_len++] = cand;
        }
        ++next_candidate;
    }
    return (cache_len >= (size_t)n) ? cache[(size_t)n - 1] : (seq_t)0;
}
