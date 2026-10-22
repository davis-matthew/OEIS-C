/*
 * A317761  Number of n X 5 0..1 arrays with every element unequal to 0, 1 or
 *          2 horizontally, vertically or antidiagonally adjacent elements,
 *          with upper left element zero.
 * https://oeis.org/A317761
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) - a(n-4) for n>6 with a(1)=16, a(2)=42, a(3)=52,
 *            a(4)=72, a(5)=109, a(6)=177 (OEIS A317761)
 * Range:     u8 n <= 6, u16 n <= 15, u32 n <= 34, u64 n <= 70, u128 n <= 143,
 *            big any n
 * Helpers:   init_val
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/init_val.h"

static seq_t *cache = NULL;          /* 1‑based sequence stored 0‑based */
static size_t cache_size = 0;        /* number of terms currently stored */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0; /* undefined, but guard */

    size_t need = (size_t)n;
    if (cache == NULL || cache_size < need) {
        size_t new_cap = need;
        seq_t *new_cache = (seq_t *)realloc(cache,
                                            new_cap * sizeof(seq_t));
        if (!new_cache) {
            /* abort on allocation failure */
            abort();
        }
        cache = new_cache;
        /* fill new entries */
        for (size_t i = cache_size; i < new_cap; ++i) {
            if (i < 6) {
                cache[i] = init_val(i);
            } else {
                /* a(n) = 2*a(n-1) - a(n-4)  (n = i+1) */
                seq_t val = cache[i - 1];
                val = val + val;           /* 2*a(n-1) */
                val = val - cache[i - 4];  /* -a(n-4) */
                cache[i] = val;
            }
        }
        cache_size = new_cap;
    }
    return cache[n - 1];
}
