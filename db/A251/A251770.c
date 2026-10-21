/*
 * A251770  Numbers n such that n^2 + (n+1)^2 + (n+2)^2 is equal to the sum of
 *          the heptagonal numbers H(m), H(m+1) and H(m+2) for some m.
 * https://oeis.org/A251770
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)+1442*a(n-2)-1442*a(n-3)-a(n-4)+a(n-5); Initial
 *            values: a(1)=27, a(2)=378, a(3)=40365, a(4)=546516,
 *            a(5)=58207743
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    if (n < 1)
        return (seq_t)0;  /* Undefined for n < 1, return 0 */

    /* static cache for computed values */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;

    /* initialise first five terms once */
    if (cache_len == 0) {
        cache_len = 5;
        cache = (seq_t*)malloc(5 * sizeof(seq_t));
        cache[0] = (seq_t)27;
        cache[1] = (seq_t)378;
        cache[2] = (seq_t)40365;
        cache[3] = (seq_t)546516;
        cache[4] = (seq_t)58207743;
    }

    /* grow cache if needed */
    if (n > cache_len) {
        cache = (seq_t*)realloc(cache, n * sizeof(seq_t));
        for (idx_t i = cache_len; i < n; ++i) {
            /* recurrence: a(i+1) computed from cache[i] etc. */
            idx_t idx = i;             /* 0‑based index for new term */
            seq_t val = cache[idx - 1];
            val = val + (seq_t)1442 * cache[idx - 2];
            val = val - (seq_t)1442 * cache[idx - 3];
            val = val - cache[idx - 4];
            val = val + cache[idx - 5];
            cache[idx] = val;
        }
        cache_len = n;
    }

    return cache[n - 1];  /* return nth term (1‑based index) */
}
