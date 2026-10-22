/*
 * A381720  Integers whose Hamming weight is a cube.
 * https://oeis.org/A381720
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   int_cube_root
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements OEIS A381720:
   a(n) is the nth integer (starting from 0) whose binary Hamming weight is a perfect cube.
   The function keeps a cache of found terms and extends it on demand; it works for
   any order of calls. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/int_cube_root.h"

/* popcount for seq_t, works for all integer widths up to 128‑bit */
static inline int popcount_seq(seq_t x)
{
    int count = 0;
    while (x)
    {
        x &= x - 1;   /* drop lowest set bit */
        ++count;
    }
    return count;
}

/* cache of terms */
static seq_t *cache       = NULL;
static size_t   cache_len = 0;
static size_t   cache_cap = 0;
static idx_t   next_k      = 0;   /* next integer to test */

static inline bool is_cube(int pc)
{
    idx_t r = int_cube_root((idx_t)pc);
    return ((r*r)*r) == (idx_t)pc;
}

static void ensure_cache_size(size_t needed)
{
    if (needed <= cache_cap)
        return;
    if (cache_cap == 0)
        cache_cap = 8;
    while (cache_cap < needed)
        cache_cap <<= 1;
    cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
}

/* main function */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* undefined, but avoid crash */

    /* extend cache until we have at least n terms */
    while (cache_len < n)
    {
        seq_t cur = (seq_t)next_k;
        int pc = popcount_seq(cur);
        if (is_cube(pc))
        {
            ensure_cache_size(cache_len + 1);
            cache[cache_len++] = cur;
        }
        ++next_k;
    }
    return cache[n - 1];
}
