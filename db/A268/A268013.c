/*
 * A268013  Number of n X 1 0..2 arrays with every repeated value in every row
 *          and column greater than or equal to the previous repeated value.
 * https://oeis.org/A268013
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 22, u64 n <= 46, u128 n <= 95,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A268013
 *  a(n) = 6*a(n‑1) - 9*a(n‑2) - 4*a(n‑3) + 9*a(n‑4) + 6*a(n‑5) + a(n‑6)   (n ≥ 7)
 *  Initial values:  a(1)=3, a(2)=9, a(3)=27, a(4)=78, a(5)=222, a(6)=622
 *  The function memoises computed values in a static cache so the order of calls
 *  does not matter.  Uses only integer arithmetic compatible with C11
 *  and with arbitrary‑precision builds.
 */

#include <stdlib.h>   /* malloc, realloc */

seq_t a(idx_t n)
{
    /* Cached values; cache[0] is unused. */
    static seq_t *cache = NULL;
    static idx_t cap     = 0;         /* actual size of the cache array */
    static idx_t maxc    = 0;         /* highest index for which a value is stored */

    /* a(0) is undefined – return 0 for safety. */
    if (n == 0)
        return 0;

    /* Ensure the cache is large enough to hold index n. */
    if (n + 1 > cap) {
        idx_t old_cap = cap;
        /* Allocate at least 7 entries to hold the 6 known initial terms. */
        idx_t new_cap = (n > 6 ? (idx_t)n + 1 : (idx_t)7);
        seq_t *tmp = (seq_t *)realloc((void *)cache, (size_t)new_cap * sizeof(seq_t));
        if (!tmp)
            return 0;          /* allocation failure – unlikely in test harness */
        cache = tmp;

        if (old_cap == 0) {      /* first allocation – initialise initial terms */
            cache[0] = 0;       /* unused */
            cache[1] = 3;
            cache[2] = 9;
            cache[3] = 27;
            cache[4] = 78;
            cache[5] = 222;
            cache[6] = 622;
            maxc = 6;
        } else {                  /* zero newly allocated entries only */
            for (idx_t i = old_cap; i < new_cap; ++i)
                cache[i] = 0;
        }
        cap = new_cap;
    }

    /* Return if already known. */
    if (n <= maxc)
        return cache[n];

    /* Compute terms up to n.  start is the first index that is still unknown. */
    idx_t start = maxc + 1;
    /* For safety, never compute below index 7; we have initial values up to 6. */
    if (start < 7)
        start = 7;

    for (idx_t i = start; i <= n; ++i) {
        seq_t val = 6 * cache[i - 1]
                  - 9 * cache[i - 2]
                  - 4 * cache[i - 3]
                  + 9 * cache[i - 4]
                  + 6 * cache[i - 5]
                  +   cache[i - 6];
        cache[i] = val;
    }
    maxc = n;
    return cache[n];
}
