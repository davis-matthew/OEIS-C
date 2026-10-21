/*
 * A287814  Number of octonary sequences of length n such that no two
 *          consecutive terms have distance 3.
 * https://oeis.org/A287814
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*a(n-1) - 10*a(n-3) with a(0)=1, a(1)=8, a(2)=54,
 *            a(3)=366
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   alloc_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/alloc_seq_array.h"

static seq_t *cache = NULL;   /* cached values */
static size_t cache_cap = 0;  /* capacity of cache */
static size_t last_computed = 3;   /* largest index computed, at least 3 */

/* Return the n-th term of A287814 */
seq_t a(idx_t n)
{
    /* Handle small indices directly */
    if (n < 4) {
        switch ((idx_t)n) {
            case 0: return (seq_t)1;
            case 1: return (seq_t)8;
            case 2: return (seq_t)54;
            case 3: return (seq_t)366;
            default: return (seq_t)0;  /* unreachable */
        }
    }

    /* Initialise cache on first use */
    if (cache == NULL) {
        cache_cap = 4;                      /* store indices 0..3 */
        cache = alloc_seq_array(cache_cap); /* returns seq_t* */
        if (cache == NULL) return (seq_t)0; /* allocation failed */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)8;
        cache[2] = (seq_t)54;
        cache[3] = (seq_t)366;
        last_computed = 3;
    }

    /* Resize cache if needed */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL)
            return (seq_t)0; /* allocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute and cache missing terms */
    for (size_t i = last_computed + 1; i <= (size_t)n; ++i) {
        cache[i] = (seq_t)((seq_t)7 * cache[i-1] - (seq_t)10 * cache[i-3]);
    }
    last_computed = (size_t)n;
    return cache[(size_t)n];
}
