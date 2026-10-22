/*
 * A357289  a(n) = number of subsets S of {1,2,...,n} having more than 2
 *          elements such that (sum of least three elements of S) > max(S).
 * https://oeis.org/A357289
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) - a(n-2) - a(n-3) - 6*a(n-4) + 2*a(n-5) +
 *            20*a(n-6) - 24*a(n-7) + 8*a(n-8) Initial terms: a(0)=0, a(1)=0,
 *            a(2)=0, a(3)=1, a(4)=5, a(5)=16, a(6)=38, a(7)=83
 * Range:     u8 n <= 8, u16 n <= 19, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


seq_t a(idx_t n)
{
    static seq_t *cache  = NULL;
    static idx_t cap     = 0;
    static idx_t max_comp = 0;

    /* Allocate the cache and initialize the first 8 terms. */
    if (cache == NULL)
    {
        cap = 8;
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        if (cache == NULL) return 0;  /* unlikely in tests */

        cache[0] = 0;
        cache[1] = 0;
        cache[2] = 0;
        cache[3] = 1;
        cache[4] = 5;
        cache[5] = 16;
        cache[6] = 38;
        cache[7] = 83;
        max_comp = 7;
    }

    /* Grow the cache if the requested index is beyond current capacity. */
    if (n >= cap)
    {
        idx_t new_cap = cap;
        while (new_cap <= n) new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return 0;  /* allocation failure */
        for (idx_t i = cap; i < new_cap; ++i) tmp[i] = 0;
        cache = tmp;
        cap = new_cap;
    }

    /* Compute missing terms up to n. */
    if (n > max_comp)
    {
        for (idx_t i = max_comp + 1; i <= n; ++i)
        {
            seq_t val = 3 * cache[i - 1]
                       - cache[i - 2]
                       - cache[i - 3]
                       - 6 * cache[i - 4]
                       + 2 * cache[i - 5]
                       + 20 * cache[i - 6]
                       - 24 * cache[i - 7]
                       + 8 * cache[i - 8];
            cache[i] = val;
        }
        max_comp = n;
    }

    return cache[n];
}
