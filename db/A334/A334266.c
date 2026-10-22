/*
 * A334266  Numbers k such that the k-th composition in standard order is both
 *          a reversed Lyndon word and a co-Lyndon word.
 * https://oeis.org/A334266
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   is_lattice, popcount_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements A334266: n‑th number k such that the k‑th composition
   in standard order is both a reversed Lyndon word and a co‑Lyndon word. */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_lattice.h"
#include "../utils/popcount_u64.h"

/* Count set bits of a 64‑bit integer (used for popcount). */

/* Return true if k has a composition that satisfies both
   reversed‑Lyndon and co‑Lyndon conditions. */
static bool qualifies(idx_t k) {
    int r = popcount_u64((uint64_t)k);        /* number of parts in composition */
    if (r == 0) return true;                 /* empty composition qualifies */

    seq_t orig[64];                          /* differences between successive positions */
    seq_t rev[64];                           /* reversed word */

    seq_t prev = 0;
    int idx = 0;
    for (int bit = 0; bit < 64; ++bit) {
        if ((k >> bit) & 1ULL) {
            seq_t pos = (seq_t)(bit + 1);
            seq_t diff = pos - prev;
            orig[idx++] = diff;
            prev = pos;
        }
    }
    /* reverse to obtain the composition w */
    for (int i = 0; i < r; ++i) rev[i] = orig[r - 1 - i];

    /* reversed‑Lyndon: original array < all rotations */
    if (!is_lattice(orig, r, true))  return false;
    /* co‑Lyndon: reversed array > all rotations */
    if (!is_lattice(rev, r, false))  return false;

    return true;
}

/* Cache of found qualifying values */
static seq_t *cache = NULL;
static size_t cache_size   = 0;
static size_t cache_cap    = 0;
static idx_t  next_k       = 0;

/* Return the n‑th term (1‑based) of sequence A334266. */
seq_t a(idx_t n) {
    size_t target = (size_t)n;
    if (cache_size < target) {
        idx_t k = next_k;
        while (cache_size < target) {
            if (qualifies(k)) {
                if (cache_size == cache_cap) {
                    size_t new_cap = cache_cap ? cache_cap << 1 : 64;
                    if (new_cap < target) new_cap = target;
                    seq_t *tmp = (seq_t *)realloc(cache,
                                                  new_cap * sizeof(seq_t));
                    cache = tmp;
                    cache_cap = new_cap;
                }
                cache[cache_size++] = (seq_t)k;
            }
            ++k;
        }
        next_k = k;
    }
    return cache[target - 1];
}
