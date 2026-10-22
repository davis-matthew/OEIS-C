/*
 * A339996  Numbers whose square is rotationally ambigrammatic with no
 *          trailing zeros.
 * https://oeis.org/A339996
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 17, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search algorithm for A339996: numbers whose square is rotationally ambigrammatic
   with no trailing zeros.  The function builds the sequence on demand using
   a linear search; it collects terms in an expanding static array, so calls
   may arrive in any order. */
#include <stdlib.h>
#include <stdbool.h>

/* Optional reallocation helper – use the build‑provided one if available. */
#ifndef seq_realloc
static void *seq_realloc(void *ptr, size_t old_cap, size_t new_cap) {
    return realloc(ptr, new_cap * sizeof(seq_t));
}
#endif

/* External linkage.  seq_t and idx_t are defined by the build system. */
seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of stored terms */
    static size_t cache_cap = 0;   /* allocated capacity */
    static idx_t next_x = 0;       /* next integer to test */

    /* Expand the cache until we have at least n terms */
    while (cache_len < n) {
        /* Treat 0 as the very first term specially */
        if (next_x == 0) {
            /* 0 is a valid value: its square is 0 which is rotationally ambigrammatic */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 64;
                cache = (seq_t *)seq_realloc(cache, cache_cap, new_cap);
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)0;
            ++next_x;
            continue;
        }

        idx_t x = next_x++;
        int d = x % 10;
        /* Exclude numbers ending in 0,2,5,8 (except for 0 handled above) */
        if (d == 0 || d == 2 || d == 5 || d == 8)
            continue;

        seq_t sq = (seq_t)x * (seq_t)x;

        /* Exclude squares that end with 0 (would create leading zeros after rotation) */
        if (sq % 10 == 0)
            continue;

        /* Check that every decimal digit of the square is in {0,1,6,8,9} */
        bool ok = true;
        seq_t t = sq;
        if (t == 0) {
            /* 0 is allowed */
        } else {
            while (t != 0) {
                unsigned dv = (unsigned)(t % 10);
                if (dv != 0 && dv != 1 && dv != 6 && dv != 8 && dv != 9) {
                    ok = false;
                    break;
                }
                t /= 10;
            }
        }
        if (!ok)
            continue;

        /* Add the valid number to the cache */
        if (cache_len == cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 64;
            cache = (seq_t *)seq_realloc(cache, cache_cap, new_cap);
            cache_cap = new_cap;
        }
        cache[cache_len++] = (seq_t)x;
    }

    /* a(1) corresponds to cache[0] */
    return cache[n - 1];
}
