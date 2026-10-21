/*
 * A290915  p-INVERT of the positive integers, where p(S) = 1 - 8*S^2.
 * https://oeis.org/A290915
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 0,8,32,144,672,… Recurrence: a(0)=0, a(1)=8, a(2)=32,
 *            a(3)=144, a(n) = 4*a(n-1) + 2*a(n-2) + 4*a(n-3) - a(n-4) for n ≥
 *            4
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 14, u64 n <= 28, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;      /* dynamically growable cache of sequence values */
static size_t cache_size = 0;    /* number of entries currently stored */

/* Helper: ensure the cache can hold indices 0..n */
static void ensure_cache(idx_t n)
{
    size_t needed = (size_t)n + 1;
    if (needed <= cache_size) return;

    seq_t *new_cache = (seq_t *)realloc(cache, needed * sizeof(seq_t));
    if (new_cache == NULL) abort();          /* out of memory */
    cache = new_cache;

    /* Initialise new entries from cache_size up to needed-1 */
    for (size_t j = cache_size; j < needed; ++j)
    {
        if (j == 0) {
            cache[j] = (seq_t)0;
        } else if (j == 1) {
            cache[j] = (seq_t)8;
        } else if (j == 2) {
            cache[j] = (seq_t)32;
        } else if (j == 3) {
            cache[j] = (seq_t)144;
        } else {
            /* recurrence using previously computed entries */
            seq_t a1 = cache[j-1];
            seq_t a2 = cache[j-2];
            seq_t a3 = cache[j-3];
            seq_t a4 = cache[j-4];
            cache[j] = (4*a1) + (2*a2) + (4*a3) - a4;
        }
    }
    cache_size = needed;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[(size_t)n];
}
