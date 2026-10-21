/*
 * A268272  Negabinary evil numbers (see comment).
 * https://oeis.org/A268272
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th non‑negative integer whose base –2
 *            representation contains an even number of 1’s
 * Range:     u8 n <= 127, u16 n <= 1999, u32 n <= 1999, u64 n <= 1999,
 *            u128 n <= 1999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Test whether a non‑negative integer x has an even number of 1’s in its
   base –2 representation. */
static bool negabinary_is_evil(idx_t x) {
    int64_t val = (int64_t)x;      /* use signed type for division by –2 */
    int parity = 0;                /* 0 = even, 1 = odd */

    if (val == 0) return true;

    while (val != 0) {
        int64_t r = val % -2;      /* remainder may be 0 or –1 */
        val /= -2;
        if (r < 0) {               /* adjust to obtain digit 0 or 1 */
            r += 2;
            val += 1;
        }
        if (r == 1)
            parity ^= 1;
    }
    return parity == 0;
}

/* Static cache for the sequence values. */
static seq_t  *cache  = NULL;     /* array of values */
static size_t cache_cap = 0;      /* allocated capacity */
static size_t cache_len = 0;      /* number of values stored */
static idx_t  next_candidate = 0; /* next integer to test */

/* Ensure the cache is large enough to hold `required` elements. */
static void grow_cache(size_t required) {
    if (required <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    while (new_cap < required)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)            /* allocation failure – unlikely for the problem size */
        return;
    cache = tmp;
    cache_cap = new_cap;
}

/* Returns the n-th negabinary evil number. */
seq_t a(idx_t n) {
    while (cache_len <= n) {
        if (negabinary_is_evil(next_candidate)) {
            grow_cache(cache_len + 1);
            cache[cache_len++] = (seq_t)next_candidate;
        }
        next_candidate++;
    }
    return cache[n];
}
