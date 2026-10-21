/*
 * A298677  a(n) = 110*a(n-1) - a(n-2) for n >= 2, a(0)=1, a(1)=111.
 * https://oeis.org/A298677
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 110 * a(n-1) - a(n-2), with a(0)=1, a(1)=111
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* External linkage function as required */
seq_t a(idx_t n) {
    /* Static cache to store computed values. */
    static seq_t *cache = NULL;
    static idx_t next = 0;      /* next index to be computed (first free slot) */

    if (cache == NULL) {
        /* Allocate space for at least 2 entries. */
        cache = (seq_t *)malloc(2 * sizeof(seq_t));
        if (!cache) return 0;   /* out‑of‑memory; caller will handle 0 */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)111;
        next = 2;              /* next index to compute */
    }

    /* Grow the cache if needed. */
    if (n >= next) {
        idx_t new_cap = next * 2;
        if (new_cap < n + 1) new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0; /* realloc failed */
        cache = tmp;
        /* Compute all missing values up to n. */
        for (idx_t i = next; i <= n; ++i) {
            seq_t prev1 = cache[i - 1];
            seq_t prev2 = cache[i - 2];
            cache[i] = (seq_t)110 * prev1 - prev2;
        }
        next = n + 1;
    }

    return cache[n];
}
