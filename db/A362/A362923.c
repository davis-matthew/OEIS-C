/*
 * A362923  G.f. = (3*x^4+5*x^2+6*x-7)/(4*x^7+x^4+x^2+x-1).
 * https://oeis.org/A362923
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=7, a(1)=1, a(2)=3, a(3)=4, a(4)=11, a(5)=16, a(6)=30,
 *            a(n)=a(n-1)+a(n-2)+a(n-4)+4*a(n-7) for n>6
 * Range:     u8 n <= 9, u16 n <= 18, u32 n <= 36, u64 n <= 73, u128 n <= 147,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t computed_up_to = 0;   /* next index not yet computed */

static void ensure_cache(idx_t n) {
    if (n < computed_up_to) return;

    size_t needed = (size_t)n + 1;
    if (needed > cache_cap) {
        size_t new_cap = needed > cache_cap * 2 ? needed : cache_cap * 2;
        if (new_cap == 0) new_cap = 8;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    for (idx_t i = computed_up_to; i <= n; ++i) {
        if (i == 0)          cache[0] = 7;
        else if (i == 1)     cache[1] = 1;
        else if (i == 2)     cache[2] = 3;
        else if (i == 3)     cache[3] = 4;
        else if (i == 4)     cache[4] = 11;
        else if (i == 5)     cache[5] = 16;
        else if (i == 6)     cache[6] = 30;
        else {
            seq_t a1 = cache[i - 1];
            seq_t a2 = cache[i - 2];
            seq_t a4 = cache[i - 4];
            seq_t a7 = cache[i - 7];
            cache[i] = a1 + a2 + a4 + a7 * (seq_t)4;
        }
    }
    computed_up_to = n + 1;
}

seq_t a(idx_t n) {
    /* lazily compute and memoize values */
    ensure_cache(n);
    return cache[n];
}
