/*
 * A278491  After a(0)=0, numbers n such that (A002828(1+n) = 1) and
 *          (A002828(4+n) = 4).
 * https://oeis.org/A278491
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 96, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   good_s
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A278491: Numbers n such that n+1 is a perfect square and n+4 is of the
   form 4^a(8b+7).  a(0)=0; for n≥1, if s is the n‑th integer with
   s²+3=4^a(8b+7) then a(n)=s²−1. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/good_s.h"

static seq_t *cache      = NULL;    /* stored terms a(0)…a(k) */
static size_t  cache_cap = 0;       /* allocated capacity */
static idx_t   term_cnt  = 0;       /* number of terms after a(0) stored */
static idx_t   last_checked = 1;    /* last s examined for condition */

/* Grow the cache if necessary to hold at least n+1 entries. */
static inline void ensure_cache(idx_t n)
{
    if (cache_cap == 0) {            /* first time: set up a(0) */
        cache_cap = 1;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        cache[0] = 0;                /* a(0) = 0 */
    }

    /* Generate terms until we have at least index n. */
    while (term_cnt < n) {           /* term_cnt counts terms after a(0) */
        last_checked++;              /* next integer to test */
        if (good_s(last_checked)) {
            /* need space for a(new_term) at index term_cnt+1 */
            if (term_cnt + 2 > cache_cap) {
                cache_cap = cache_cap ? cache_cap * 2 : 2;
                cache = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
            }
            seq_t val = ((seq_t)last_checked * last_checked) - 1;
            cache[term_cnt + 1] = val;
            term_cnt++;              /* one more term added */
        }
    }
}

/* Public interface required by the build. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    ensure_cache(n);
    return cache[n];
}
