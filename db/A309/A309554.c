/*
 * A309554  a(1) = a(6) = 1, a(2) = a(3) = a(8) = 2, a(4) = a(7) = 7, a(5) =
 *          5; a(n) = a(n-a(n-1)) + a(n-a(n-3)) for n > 8.
 * https://oeis.org/A309554
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-a(n-1)) + a(n-a(n-3)) with initial values: a(1)=1,
 *            a(2)=2, a(3)=2, a(4)=7, a(5)=5, a(6)=1, a(7)=7, a(8)=2 Offset: 1
 *            Implementation uses lazy memoisation for arbitrary call order
 * Range:     u8 n <= 255, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache for already computed terms: index 1..cap-1 inclusive */
    static seq_t *cache = NULL;
    static idx_t cap = 0;          /* current size of cache array */

    if (n == 0) return 0;          /* safeguard; not used by the sequence */

    if (n >= cap) {                /* need to grow and compute up to n */
        idx_t new_cap = n + 1;     /* we store up to index n inclusive */
        if (cache) {
            cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        } else {
            cache = (seq_t*)malloc(new_cap * sizeof(seq_t));
        }
        /* fill new entries; start at the smallest index that hasn't yet
           been computed.  If cap==0 we start at 1. */
        idx_t start = (cap > 1) ? cap : 1;
        for (idx_t i = start; i <= n; ++i) {
            if (i == 1) cache[1] = 1;
            else if (i == 2) cache[2] = 2;
            else if (i == 3) cache[3] = 2;
            else if (i == 4) cache[4] = 7;
            else if (i == 5) cache[5] = 5;
            else if (i == 6) cache[6] = 1;
            else if (i == 7) cache[7] = 7;
            else if (i == 8) cache[8] = 2;
            else {
                /* recurrence: a(i) = a(i - a(i-1)) + a(i - a(i-3)) */
                idx_t idx1 = i - (idx_t)cache[i-1];
                idx_t idx2 = i - (idx_t)cache[i-3];
                cache[i] = cache[idx1] + cache[idx2];
            }
        }
        cap = new_cap;             /* update cache size */
    }
    return cache[n];
}
