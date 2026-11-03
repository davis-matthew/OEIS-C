/*
 * A297257  Numbers whose base-5 digits have greater up-variation than
 *          down-variation; see Comments.
 * https://oeis.org/A297257
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 113, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 75 terms through n = 9862 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) is the n‑th integer whose base‑5 digits have
   greater up‑variation than down‑variation.  The function
   computes terms by scanning integers in increasing order and
   caching previously found values.  */

/*=== includes ===*/
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/mem.h"

/*=== helper to test the condition for a candidate number ===*/
static bool base5_up_gt_down(idx_t n)
{
    /* digits in reverse order (least significant first) */
    unsigned char digits[64];
    size_t dcnt = 0;
    while (n > 0) {
        digits[dcnt++] = n % 5;
        n /= 5;
    }
    if (dcnt == 0) {
        /* n == 0, not considered */
        return false;
    }

    /* iterate from most significant to least */
    int uv = 0;      /* up‑variation */
    int dv = 0;      /* down‑variation */
    for (size_t i = dcnt - 1; i > 0; --i) {
        int prev = digits[i];
        int cur  = digits[i - 1];
        if (cur > prev) {
            uv += cur - prev;
        } else if (cur < prev) {
            dv += prev - cur;
        }
    }
    return uv > dv;
}

/*=== memoized table of sequence values ===*/
static seq_t *cache = NULL;       /* array of found a(k) values */
static size_t cache_len = 0;      /* number of elements currently stored */
static size_t cache_cap = 0;      /* allocated capacity of cache */
static idx_t   last_checked = 0;  /* last integer examined */

/*=== main sequence function ===*/
seq_t a(idx_t n)
{
    if (n == 0) {
        /* offset 1; behaviour for n<1 is undefined */
        return (seq_t)0;
    }

    /* extend cache until it contains at least n elements */
    while (cache_len < n) {
        /* move to next candidate */
        ++last_checked;
        if (base5_up_gt_down(last_checked)) {
            /* ensure capacity */
            if (cache_len == cache_cap) {
                size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
                seq_t *new_cache = mem_resize(seq_t, cache, new_cap);
                if (!new_cache) {
                    /* Allocation failed – abort with 0 (shouldn't happen) */
                    return (seq_t)0;
                }
                cache = new_cache;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)last_checked;
        }
    }

    return cache[n - 1];
}
