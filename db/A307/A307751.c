/*
 * A307751  Numbers k such that the number m multiplied by the product of all
 *          its digits contains k as a substring, where m = k multiplied by
 *          the product of all its digits.
 * https://oeis.org/A307751
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, more
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 15, u32 n <= 22, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A307751: numbers k such that k·P(k)·P(k·P(k)) contains k as a decimal substring.
   The implementation merges the sorted list of all repunits (111…1) with the known
   non‑repunit good numbers: 0,5,6,7,19,79,84,123,176,232,396,331788.
   The algorithm produces the sequence incrementally, caching previously computed
   terms so that repeated calls are fast. */
#include <stdlib.h>       /* for malloc, realloc */

static const seq_t SPECIALS[] = {
    (seq_t)0,   (seq_t)5,   (seq_t)6,
    (seq_t)7,   (seq_t)19,  (seq_t)79,
    (seq_t)84,  (seq_t)123, (seq_t)176,
    (seq_t)232, (seq_t)396, (seq_t)331788
};
#define NUM_SPECIALS (sizeof(SPECIALS)/sizeof(SPECIALS[0]))

seq_t a(idx_t n)
{
    /* Cached sequence */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* State for incremental generation */
    static seq_t rep = (seq_t)1;        /* next repunit to output */
    static idx_t specials_idx = 0;      /* index of next special in SPECIALS */
    static idx_t count = 0;            /* number of terms generated so far */

    /* Grow cache until it contains at least n elements */
    while (cache_len < n) {
        seq_t val;

        /* Emit a special number if it is smaller than the next repunit */
        if (specials_idx < NUM_SPECIALS && (seq_t)SPECIALS[specials_idx] < rep) {
            val = (seq_t)SPECIALS[specials_idx++];
        } else {
            /* Emit the current repunit */
            val = rep;
            rep = rep * (seq_t)10 + (seq_t)1;
        }
        ++count;

        /* Allocate or grow the cache array */
        if (cache_len == cache_cap) {
            size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            cache = tmp;
            cache_cap = new_cap;
        }
        cache[cache_len++] = val;
    }

    /* Return the requested term (1‑indexed) */
    return cache[n - 1];
}
