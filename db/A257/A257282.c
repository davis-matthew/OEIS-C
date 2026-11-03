/*
 * A257282  Numbers whose square is not the sum of two consecutive nonsquares.
 * https://oeis.org/A257282
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n even or ((n^2-1)/2 or (n^2+1)/2 is a perfect square)
 * Range:     u8 n <= 10, u16 n <= 65, u32 n <= 10001, u64 n <= 10001,
 *            u128 n <= 10001, big any n
 * Validated: standard, pass, 81 terms through n = 9888 (2026-11-03)
 * Helpers:   ipow_i32, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by filtering numbers until the n‑th term is reached. */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/ipow_i32.h"
#include "../utils/mem.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

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
                seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                if (!tmp) {            /* allocation failed */
                    mem_free(cache);
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
