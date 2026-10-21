/*
 * A251003  Number of (n+1) X (1+1) 0..2 arrays with no 2 X 2 subblock having
 *          its maximum diagonal element less than the absolute difference of
 *          its antidiagonal elements.
 * https://oeis.org/A251003
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 9*a(n-1) - 12*a(n-2) + 17*a(n-3) - 12*a(n-4) + 4*a(n-5)
 *            for n>5; initial terms: a(1)=69, a(2)=530, a(3)=4083,
 *            a(4)=31456, a(5)=242326
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 20, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cached_n = 0;
    if (n == 0) return 0; /* not part of the sequence */

    if (n <= cached_n) {
        return cache[n-1];
    }

    size_t new_cap = (size_t)n;
    if (cache == NULL) {
        cache = (seq_t*)malloc(sizeof(seq_t) * new_cap);
        if (!cache) return 0; /* allocation failed */
        cached_n = 0;
    } else if (new_cap > cached_n) {
        seq_t *tmp = (seq_t*)realloc(cache, sizeof(seq_t) * new_cap);
        if (!tmp) return 0; /* allocation failed */
        cache = tmp;
    }

    for (idx_t k = cached_n + 1; k <= n; ++k) {
        idx_t idx = k - 1; /* zero‑based index */
        if (k == 1) {
            cache[idx] = (seq_t)69;
        } else if (k == 2) {
            cache[idx] = (seq_t)530;
        } else if (k == 3) {
            cache[idx] = (seq_t)4083;
        } else if (k == 4) {
            cache[idx] = (seq_t)31456;
        } else if (k == 5) {
            cache[idx] = (seq_t)242326;
        } else {
            cache[idx] = (seq_t)9 * cache[idx-1]
                      - (seq_t)12 * cache[idx-2]
                      + (seq_t)17 * cache[idx-3]
                      - (seq_t)12 * cache[idx-4]
                      + (seq_t)4 * cache[idx-5];
        }
    }
    cached_n = n;
    return cache[n-1];
}
