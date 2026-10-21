/*
 * A254654  Pentagonal numbers (A000326) which are also centered heptagonal
 *          numbers (A069099).
 * https://oeis.org/A254654
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)+12098*a(n-2)-12098*a(n-3)-a(n-4)+a(n-5); * Initial
 *            values: a(1)=1, a(2)=22, a(3)=11572, a(4)=265651, a(5)=139997551
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 10, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static idx_t cache_n = 0;
    /* fixed initial values (offset 1) */
    static const seq_t init_vals[5] = {
        (seq_t)1,
        (seq_t)22,
        (seq_t)11572,
        (seq_t)265651,
        (seq_t)139997551
    };

    if (n <= 5) {
        return init_vals[n - 1];
    }

    /* initialize cache with the first five values if not done */
    if (cache == NULL) {
        cache = (seq_t *)malloc(5 * sizeof(seq_t));
        if (!cache) abort();          /* allocation failure */
        for (idx_t i = 0; i < 5; ++i) cache[i] = init_vals[i];
        cache_n = 5;
    }

    /* grow cache up to index n (1‑based) if needed */
    if (n > cache_n) {
        seq_t *tmp = (seq_t *)realloc(cache, n * sizeof(seq_t));
        if (!tmp) abort();            /* allocation failure */
        cache = tmp;
        for (idx_t i = cache_n; i < n; ++i) {
            /* recurrence: a(i+1) = a(i) + 12098*a(i-1) - 12098*a(i-2) - a(i-3) + a(i-4) */
            /* zero‑based: cache[i-1] = a(i), cache[i-2] = a(i-1), ... */
            seq_t term = cache[i - 1] + cache[i - 5];
            term += (seq_t)12098 * (cache[i - 2] - cache[i - 3]);
            term -= cache[i - 4];
            cache[i] = term;
        }
        cache_n = n;
    }

    return cache[n - 1];
}
