/*
 * A301786  Number of nX4 0..1 arrays with every element equal to 0 or 1
 *          horizontally or antidiagonally adjacent elements, with upper left
 *          element zero.
 * https://oeis.org/A301786
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 2*a(n-2) - a(n-4) for n>=5; initial values:
 *            a(1)=5, a(2)=9, a(3)=17, a(4)=32
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 33, u64 n <= 67, u128 n <= 136,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t last_computed = 0;

static void ensure_cache_size(idx_t n) {
    if (cache_cap > n) return;
    size_t new_cap = cache_cap ? cache_cap : 4;
    while (new_cap <= (size_t)n) new_cap <<= 1;
    cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;                      /* undefined; safe fallback */
    ensure_cache_size(n);
    if (n > last_computed) {
        for (idx_t i = last_computed + 1; i <= n; ++i) {
            if (i == 1) {
                cache[i] = (seq_t)5;
            } else if (i == 2) {
                cache[i] = (seq_t)9;
            } else if (i == 3) {
                cache[i] = (seq_t)17;
            } else if (i == 4) {
                cache[i] = (seq_t)32;
            } else {
                cache[i] = cache[i-1] + (seq_t)2 * cache[i-2] - cache[i-4];
            }
        }
        last_computed = n;
    }
    return cache[n];
}
