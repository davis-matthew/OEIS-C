/*
 * A397120  Size of Type A Megastars of star triples in the Spectre aperiodic
 *          monotile tiling.
 * https://oeis.org/A397120
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) + 15*a(n-2) + a(n-3) for n>3; a(1) = 1, a(2) =
 *            4, a(3) = 25
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 26, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_size = 0;

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;

    /* Ensure the cache contains values up to n. */
    size_t required = (size_t)n;
    if (required > cache_size) {
        size_t new_size = required;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;

        /* Initialize new entries. */
        for (size_t i = cache_size; i < new_size; ++i) {
            if (i == 0)                cache[i] = (seq_t)1;   /* a(1) */
            else if (i == 1)           cache[i] = (seq_t)4;   /* a(2) */
            else if (i == 2)           cache[i] = (seq_t)25;  /* a(3) */
            else
                cache[i] = 3 * cache[i - 1] + 15 * cache[i - 2] + cache[i - 3];
        }
        cache_size = new_size;
    } else if (cache_size < 3) { /* Cache exists but has fewer than 3 entries */
        size_t needed = 3;
        if (needed > cache_size) {
            seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
            if (!tmp) abort();
            cache = tmp;
            for (size_t i = cache_size; i < needed; ++i) {
                if (i == 0)                cache[i] = (seq_t)1;   /* a(1) */
                else if (i == 1)           cache[i] = (seq_t)4;   /* a(2) */
                else if (i == 2)           cache[i] = (seq_t)25;  /* a(3) */
            }
            cache_size = needed;
        }
    }

    return cache[(size_t)(n - 1)];
}
