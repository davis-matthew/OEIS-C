/*
 * A281130  a(n+1) = a(n-a(n)) if a(n-1) < a(n), otherwise a(n+1) = 2*a(n);
 *          a(1) = a(2) = 1.
 * https://oeis.org/A281130
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n+1) = (a(n-1) < a(n)) ? a(n-a(n)) : 2*a(n); a(1) = a(2) = 1
 * Range:     u8 n <= 436, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


static seq_t *cache = NULL;
static idx_t  cache_size = 0;

static void ensure_cache(idx_t n) {
    if (n <= cache_size) return;
    idx_t needed = n + 1;                 /* allocate indices 0 .. n (0 unused) */
    seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);
    cache = tmp;

    for (idx_t i = cache_size + 1; i <= n; ++i) {
        if (i == 1) {
            cache[1] = 1;
        } else if (i == 2) {
            cache[2] = 1;
        } else {
            seq_t prev1 = cache[i-1];
            seq_t prev2 = cache[i-2];
            if (prev2 < prev1) {
                idx_t idx = (idx_t)(i-1) - (idx_t)prev1;
                cache[i] = cache[idx];
            } else {
                cache[i] = prev1 + prev1;   /* 2*prev1 */
            }
        }
    }
    cache_size = n;
}

seq_t a(idx_t n) {
    if (n < 1) return 0;
    ensure_cache(n);
    return cache[n];
}
