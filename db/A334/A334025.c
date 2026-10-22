/*
 * A334025  a(0)=0, a(1)=1; and a(n) = {2*a(n-2), 2*a(n-1)}, where {x,y} is
 *          the concatenation of x and y.
 * https://oeis.org/A334025
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(0)=0, a(1)=1, a(n)=concat(2*a(n-2),2*a(n-1)) for n ≥ 2
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 6, u64 n <= 7, u128 n <= 9,
 *            big any n
 * Helpers:   digit_len, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/pow10.h"
#include "../utils/digit_len.h"

static seq_t *cache      = NULL;   /* stored values */
static size_t cache_size = 0;      /* how many values stored */
static size_t cache_cap  = 0;      /* allocated array capacity */

/* grow cache so that it contains terms up to index n */
static void ensure_cache(idx_t n)
{
    /* enlarge array if needed */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 4;
        while (new_cap <= (size_t)n + 1) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache     = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms */
    while (cache_size <= (size_t)n) {
        idx_t i = (idx_t)cache_size;
        seq_t val;

        if (i == 0) {
            val = (seq_t)0;
        } else if (i == 1) {
            val = (seq_t)1;
        } else {
            seq_t x = cache[i - 2];          /* a(i-2) */
            seq_t y = cache[i - 1];          /* a(i-1) */
            seq_t two_x = (seq_t)2 * x;      /* 2*a(i-2) */
            seq_t two_y = (seq_t)2 * y;      /* 2*a(i-1) */
            seq_t pow10_val = pow10((idx_t)digit_len(two_y));
            val = two_x * pow10_val + two_y; /* concat(2x, 2y) */
        }

        cache[cache_size] = val;
        cache_size++;
    }
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[(size_t)n];
}
