/*
 * A320730  Integers k such that A086747(k) = 0, where A086747 is the
 *          Baum-Sweet sequence.
 * https://oeis.org/A320730
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   has_odd_zero_run
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the nth positive integer k such that A086747(k) = 0.
   That is, k’s binary expansion contains at least one run of zeros
   of odd length. */
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/has_odd_zero_run.h"

static seq_t *cache = NULL;          /* cached a(n) values */
static size_t cache_size = 0;        /* number of values stored in cache */
static size_t cache_cap   = 0;        /* current capacity of cache */
static uint64_t last_checked = 0;    /* the largest integer k already examined */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* offset is 1, but guard against zero */

    /* grow cache until it contains at least n elements */
    while (cache_size < (size_t)n) {
        ++last_checked;

        if (has_odd_zero_run(last_checked)) {
            /* ensure space */
            if (cache_size == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 8;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) exit(1); /* allocation failure */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_size++] = (seq_t)last_checked;
        }
    }

    return cache[(size_t)(n - 1)];
}
