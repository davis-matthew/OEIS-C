/*
 * A333943  Numbers k such that the k-th composition in standard order is a
 *          reversed necklace.
 * https://oeis.org/A333943
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n‑th integer k (n ≥ 1) such that the k‑th composition
 *            in standard order is a reversed necklace. The value is obtained
 *            by a search; no closed formula is known
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Validated: partial, pass, 63 terms through n = 63 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Test whether array a[0..len-1] is lexicographically minimal
   among all its cyclic rotations (i.e., a necklace). */
static bool is_min_rotation(const seq_t *a, size_t len)
{
    if (len <= 1)
        return true;

    for (size_t shift = 1; shift < len; ++shift) {
        for (size_t j = 0; j < len; ++j) {
            seq_t x = a[(shift + j) % len];
            seq_t y = a[j];
            if (x < y)          /* found a strictly smaller rotation */
                return false;
            if (x > y)
                break;          /* current shift > original, no need to continue */
        }
    }
    return true;
}

/* Determine whether the k‑th composition in standard order
   is a reversed necklace.  The composition is built by:
   - taking the set of positions of 1's in the reversed binary
     expansion of k (1‑based positions),
   - prepending 0,
   - taking first differences,
   - reversing the differences.
   The resulting sequence must be a reversed necklace, i.e.
   its reversal must be a necklace. */
static bool is_revnecklace(idx_t k)
{
    /* positions of 1's in reversed binary expansion of k */
    seq_t pos[64];
    size_t m = 0;
    idx_t x = k;
    idx_t p = 1;
    while (x) {
        if (x & 1ULL)
            pos[m++] = (seq_t)p;
        ++p;
        x >>= 1;
    }

    /* first differences (with leading 0) */
    seq_t diffs[64];
    size_t d = 0;
    seq_t prev = 0;
    for (size_t i = 0; i < m; ++i) {
        diffs[d++] = pos[i] - prev;
        prev = pos[i];
    }

    /* composition = reverse of diffs */
    seq_t comp[64];
    for (size_t i = 0; i < d; ++i)
        comp[i] = diffs[d - 1 - i];

    /* reversal of the composition */
    seq_t rev[64];
    for (size_t i = 0; i < d; ++i)
        rev[i] = comp[d - 1 - i];

    return is_min_rotation(rev, d);
}

/* static cache of found k values */
static seq_t *cache = NULL;      /* array of k values */
static size_t cache_len = 0;     /* number of cached values */
static size_t cache_cap = 0;     /* allocated capacity */
static idx_t next_k = 1;         /* next k to test */

/* Grow the cache until it contains at least 'need' values. */
static void ensure_cache(size_t need)
{
    while (cache_len < need) {
        if (is_revnecklace(next_k)) {
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                if (!tmp)            /* allocation failure – abort */
                    return;
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)next_k;
        }
        ++next_k;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                  /* undefined for n=0 */

    if (!cache) {                  /* first call, allocate initial buffer */
        cache = mem_new(seq_t, 16);
        cache_cap = 16;
    }

    ensure_cache((size_t)n);
    return cache[n - 1];
}
