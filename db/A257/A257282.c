/*
 * A257282  Numbers whose square is not the sum of two consecutive nonsquares.
 * https://oeis.org/A257282
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n even or ((n^2-1)/2 or (n^2+1)/2 is a perfect square)
 * Range:     u8 n <= 10, u16 n <= 65, u32 n <= 9888, u64 n <= 9888,
 *            u128 n <= 9888, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by filtering numbers until the n‑th term is reached. */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/floor_sqrt.h"   /* provides seq_t floor_sqrt(seq_t) */

static bool is_square(seq_t x)
{
    seq_t r = floor_sqrt(x);
    return r * r == x;
}

seq_t a(idx_t n)
{
    /* static cache of the sequence */
    static seq_t *cache = NULL;
    static size_t  cache_len = 0;
    static size_t  cache_cap = 0;
    static idx_t   next = 0;      /* next number to test */

    /* grow the cache to hold at least n terms */
    while (cache_len < n) {
        seq_t val = (seq_t)next;
        bool include = false;

        if ((val & 1) == 0) {           /* even numbers are always included */
            include = true;
        } else {                        /* odd number */
            seq_t nn = val * val;          /* n^2  */
            seq_t x  = (nn - 1) / 2;       /* (n^2-1)/2  */
            seq_t y  = x + 1;              /* (n^2+1)/2  */
            if (is_square(x) || is_square(y)) {
                include = true;
            }
        }

        if (include) {
            if (cache_len == cache_cap) {
                size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) {            /* allocation failed */
                    free(cache);
                    exit(1);
                }
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = val;
        }
        ++next;   /* advance to next candidate */
    }

    return cache[n - 1];
}
