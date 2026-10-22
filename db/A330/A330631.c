/*
 * A330631  a(n+1) = a(n-a(n))+1 if a(n) > a(n-1), otherwise a(n+1) = 2*a(n);
 *          a(1) = a(2) = 1.
 * https://oeis.org/A330631
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1, a(2) = 1; for n ≥ 1 if a(n) > a(n-1) then a(n+1) = a(n
 *            - a(n)) + 1 else a(n+1) = 2 * a(n)
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t   cap = 0;   /* allocated length of cache */
    static idx_t    max = 0;   /* number of terms currently stored */

    /* initialise cache with the first two terms */
    if (!cache) {
        cap = 16;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return 0;              /* out of memory: undefined behaviour */
        cache[0] = (seq_t)1;               /* a(1) */
        cache[1] = (seq_t)1;               /* a(2) */
        max = 2;                           /* indices 1‑based, current highest = 2 */
    }

    /* compute terms up to the requested index n */
    while (max < n) {
        /* we need a(max+1); cache[max-1] = a(max), cache[max-2] = a(max-1) */
        seq_t curr = cache[max - 1];
        seq_t prev = cache[max - 2];

        seq_t next;
        if (curr > prev) {
            /* a(max+1) = a(max - a(max)) + 1 */
            idx_t target = max - (idx_t)curr;  /* 1‑based index of a(max - a(max)) */
            next = cache[target - 1] + (seq_t)1;
        } else {
            /* a(max+1) = 2 * a(max) */
            next = curr + curr;
        }

        /* grow cache if necessary */
        if (max >= cap) {
            size_t new_cap = cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return 0;            /* out of memory: undefined behaviour */
            cache = tmp;
            cap = new_cap;
        }

        cache[max] = next;
        ++max;
    }

    return cache[n - 1];
}
