/*
 * A347468  Numbers k such that floor(k*sqrt(3)) = floor(h*sqrt(2)) for some
 *          h.
 * https://oeis.org/A347468
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Uses the intersection of the Beatty sequences for √3 and √2. k
 *            is in the sequence iff ∃h such that floor(k*√3) = floor(h*√2).
 *            The algorithm merges the two increasing floor sequences, storing
 *            the encountered k-values in a cache for subsequent queries
 * Range:     u8 n <= 2, u16 n <= 50, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   ensure_capacity, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/ensure_capacity.h"
#include "../utils/floor_sqrt.h"

static bool initialized = false;
static idx_t  next_k = 0, next_h = 0;     /* current indices in Beatty sequences */
static idx_t  valA   = 0, valB = 0;       /* current floor values */
static seq_t *cache  = NULL;             /* cached k-values */
static size_t cache_size = 0;            /* number of values stored */
static size_t cache_cap  = 0;            /* capacity of the cache buffer */

static void init_sequence(void)
{
    next_k = 1;
    next_h = 1;
    valA   = (idx_t)floor_sqrt((seq_t)3 * (seq_t)next_k * (seq_t)next_k);
    valB   = (idx_t)floor_sqrt((seq_t)2 * (seq_t)next_h * (seq_t)next_h);

    cache_cap  = 16;
    cache_size = 0;
    cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    /* In the unlikely event of allocation failure we let malloc fail
       naturally and crash on the subsequent use. */
    initialized = true;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* undefined, but harmless */
    if (!initialized) init_sequence();

    /* Grow cache until we have at least n entries. */
    while (cache_size < n) {
        if (valA < valB) {
            ++next_k;
            valA = (idx_t)floor_sqrt((seq_t)3 * (seq_t)next_k * (seq_t)next_k);
        } else if (valB < valA) {
            ++next_h;
            valB = (idx_t)floor_sqrt((seq_t)2 * (seq_t)next_h * (seq_t)next_h);
        } else { /* equal */
            if (cache_size == cache_cap)
                ensure_capacity(cache_size + 1, &cache_cap, (void **)&cache, sizeof(seq_t));

            cache[cache_size++] = (seq_t)next_k;   /* store the k value */

            ++next_k;
            valA = (idx_t)floor_sqrt((seq_t)3 * (seq_t)next_k * (seq_t)next_k);
            ++next_h;
            valB = (idx_t)floor_sqrt((seq_t)2 * (seq_t)next_h * (seq_t)next_h);
        }
    }
    return cache[n - 1];
}
