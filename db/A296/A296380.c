/*
 * A296380  Number of n X 2 0..1 arrays with each 1 horizontally, vertically
 *          or antidiagonally adjacent to 1 or 2 neighboring 1s.
 * https://oeis.org/A296380
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) - a(n-2) + 6*a(n-3) - 6*a(n-4) + 6*a(n-5) -
 *            5*a(n-6) + 3*a(n-7) - a(n-8) (empirical) Initial terms: a(1)=2,
 *            a(2)=10, a(3)=28, a(4)=86, a(5)=279, a(6)=869, a(7)=2728,
 *            a(8)=8596
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 38, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

static const seq_t init[8] = {
    (seq_t)2, (seq_t)10, (seq_t)28, (seq_t)86,
    (seq_t)279, (seq_t)869, (seq_t)2728, (seq_t)8596
};

static seq_t *cache = NULL;
static size_t cache_cap = 0;   /* allocated capacity */
static size_t cache_size = 0;  /* number of entries currently computed */

static void ensure_cache_size(idx_t n)
{
    if (cache_cap < (size_t)n) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 16;
        while (new_cap < (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();          /* out of memory */
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* Compute a(n) with memoisation.  Calls may come in any order. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1 – undefined for n<1 */

    ensure_cache_size(n);

    /* initialise first 8 terms if required */
    size_t init_needed = (size_t)n < 8 ? (size_t)n : 8;
    if (cache_size < init_needed) {
        for (idx_t i = cache_size; i < init_needed; ++i)
            cache[i] = init[i];
        if (cache_size < init_needed) cache_size = init_needed;
    }

    /* compute remaining terms */
    for (idx_t i = cache_size; i < n; ++i) {
        seq_t term =   (seq_t)3 * cache[i-1]
                     -            cache[i-2]
                     + (seq_t)6 * cache[i-3]
                     - (seq_t)6 * cache[i-4]
                     + (seq_t)6 * cache[i-5]
                     - (seq_t)5 * cache[i-6]
                     + (seq_t)3 * cache[i-7]
                     -            cache[i-8];
        cache[i] = term;
    }

    if (cache_size < (size_t)n) cache_size = n;
    return cache[n-1];
}
