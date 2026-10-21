/*
 * A292453  Numbers where 3 outnumbers any other digit.
 * https://oeis.org/A292453
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   digit_counts
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A292453: numbers where digit 3 appears more times than any other digit. */

/* The implementation searches integers in increasing order, checks the
   digit frequency using `digit_counts`, and caches the found terms so
   that subsequent calls can be answered efficiently, even when the
   index values are not monotonic.  The cache is expanded only as
   necessary. */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/digit_counts.h"

static seq_t *cache = NULL;          /* array of terms */
static size_t cache_size = 0;        /* number of cached terms */
static size_t cache_cap = 0;         /* allocated capacity of cache */
static seq_t next_start = 1;         /* next integer to examine */

/* Append a candidate value to the cache, expanding it if needed. */
static void
append_to_cache(seq_t val)
{
    if (cache_size == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        seq_t *new_arr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_arr) { /* allocation failure: terminate caller */
            /* In the context of the problem a panic is acceptable; the
               build system will handle such failure without crashing. */
            abort();
        }
        cache = new_arr;
        cache_cap = new_cap;
    }
    cache[cache_size++] = val;
}

seq_t
a(idx_t n)
{
    if (n == 0) /* undefined; return 0 as a placeholder */
        return 0;

    /* Ensure the cache contains at least n terms. */
    while (cache_size < (size_t)n) {
        unsigned char cnt[10];
        digit_counts(next_start, cnt);

        /* cnt[3] must be strictly greater than any other digit count. */
        bool good = true;
        for (int d = 0; d < 10 && good; ++d) {
            if (d == 3)
                continue;
            if (cnt[3] <= cnt[d])
                good = false;
        }

        if (good)
            append_to_cache(next_start);

        ++next_start;
    }

    return cache[n - 1];
}
