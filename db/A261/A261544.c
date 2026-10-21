/*
 * A261544  a(n) = Sum_{k=0..n} 1000^k.
 * https://oeis.org/A261544
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = (1000^(n + 1) - 1) / 999
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 3, u64 n <= 6, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 1, a(1) = 1001, a(n) = 1001*a(n-1) - 1000*a(n-2) */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cap   = 0;          /* allocated capacity */
    static idx_t size  = 0;          /* number of elements computed */

    /* initial allocation */
    if (cap == 0) {
        cap   = 1;
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (!cache) return (seq_t)0;  /* out‑of‑memory: return 0 */
        cache[0] = (seq_t)1;          /* a(0) */
        size  = 1;
    }

    /* grow cache while needed */
    while (n >= cap) {
        idx_t new_cap = cap * 2;
        if (new_cap <= n) new_cap = n + 1;

        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;    /* out‑of‑memory: return 0 */
        cache = tmp;
        cap   = new_cap;

        /* compute terms from size to cap-1 */
        for (idx_t i = size; i < cap; ++i) {
            if (i == 0) {
                cache[0] = (seq_t)1;            /* a(0) */
            } else if (i == 1) {
                cache[1] = (seq_t)1001;         /* a(1) */
            } else {
                cache[i] = cache[i-1] * (seq_t)1001
                          - cache[i-2] * (seq_t)1000;
            }
        }
        size = cap;
    }

    return cache[n];
}
