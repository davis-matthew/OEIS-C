/*
 * A255935  Triangle read by rows: a(n) = Pascal's triangle A007318(n) +
 *          A197870(n+1).
 * https://oeis.org/A255935
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = flattening of the triangle with T(0,0)=0, T(1,0)=1,
 *            T(1,1)=2, T(r,k)=T(r-1,k)+T(r-2,k-1)+T(r-2,k-2) (0 ≤ k ≤ r)
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   alloc_seq, free_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

/* The helpers below provide a portable allocator for seq_t objects
   that works with both scalar integer types and arbitrary‑precision
   integers. */
#include "../utils/alloc_seq.h"
#include "../utils/free_cache.h"

static seq_t *cache        = NULL;   /* flattened a(n) values          */
static size_t cache_cap    = 0;      /* current capacity in cache      */
static idx_t  cache_len    = 0;      /* number of values stored         */

static seq_t *prev  = NULL;          /* (r-1)-th row                   */
static size_t prev_len  = 0;
static seq_t *prev2 = NULL;          /* (r-2)-th row                   */
static size_t prev2_len = 0;
static idx_t  next_row = 0;          /* next row index to generate      */

static void grow_cache(size_t need)
{
    if (need > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = alloc_seq(new_cap);
        if (cache && cache_len) {
            memcpy(tmp, cache, cache_len * sizeof(seq_t));
            free_cache(cache);
        }
        cache = tmp;
        cache_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    /* Ensure the cache contains all values up to n. */
    while (cache_len <= n) {
        idx_t r = next_row;                 /* row number         */
        size_t curr_len = (size_t)(r + 1);   /* entries in this row */

        seq_t *curr = alloc_seq(curr_len);
        if (!curr) return (seq_t)0;         /* unlikely for tests */

        if (r == 0) {
            curr[0] = (seq_t)0;
        } else if (r == 1) {
            curr[0] = (seq_t)1;
            curr[1] = (seq_t)2;
        } else {
            for (size_t k = 0; k < curr_len; ++k) {
                seq_t v = (seq_t)0;
                if (k < prev_len)              v += prev[k];
                if (k >= 1 && k - 1 < prev2_len) v += prev2[k - 1];
                if (k >= 2 && k - 2 < prev2_len) v += prev2[k - 2];
                curr[k] = v;
            }
        }

        /* Append the new row into the cache. */
        grow_cache(cache_len + curr_len);
        memcpy(cache + cache_len, curr, curr_len * sizeof(seq_t));
        cache_len += curr_len;

        /* Move rows for next iteration. */
        prev2 = prev;
        prev2_len = prev_len;
        prev = curr;
        prev_len = curr_len;

        ++next_row;
    }

    return cache[(size_t)n];
}
