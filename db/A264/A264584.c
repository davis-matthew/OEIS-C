/*
 * A264584  Number of (1+1) X (n+1) arrays of permutations of 0..n*2+1 with
 *          each element having directed index change 1,0 0,-1 1,2 or -1,1.
 * https://oeis.org/A264584
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-3) + a(n-4) + 2*a(n-5) - a(n-9) - a(n-10); initial
 *            values:
 *            a(1)=0,a(2)=1,a(3)=0,a(4)=1,a(5)=1,a(6)=1,a(7)=3,a(8)=1,a(9)=3,a(10)=6
 * Range:     u8 n <= 22, u16 n <= 40, u32 n <= 76, u64 n <= 148,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t  cap   = 0;   /* max index currently allocated (+1) */
    static idx_t  last  = 0;   /* last index that has a valid value */

    if (n < 1) return 0;  /* undefined for n < 1 */

    /* Grow the cache if needed */
    if (n + 1 > cap) {
        idx_t new_cap = cap ? cap << 1 : n + 1;
        if (new_cap < n + 1) new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        /* If the allocation failed, return 0 (undefined behavior otherwise) */
        if (!tmp) return 0;
        /* Zero-initialize the newly added part */
        for (idx_t i = cap; i < new_cap; ++i) tmp[i] = (seq_t)0;
        cache = tmp;
        cap = new_cap;
    }

    /* Initialize known base values up to index 10 if not already done */
    if (last < 10) {
        static const seq_t base[10] = { (seq_t)0, (seq_t)1, (seq_t)0, (seq_t)1,
                                        (seq_t)1, (seq_t)1, (seq_t)3, (seq_t)1,
                                        (seq_t)3, (seq_t)6 };
        idx_t up = n < 10 ? n : 10;
        for (idx_t i = last + 1; i <= up; ++i) {
            cache[i] = base[i - 1];
        }
        last = up;
        if (n <= 10) return cache[n];
    }

    /* Compute missing terms up to n using the recurrence */
    for (idx_t i = last + 1; i <= n; ++i) {
        cache[i] = cache[i - 3] + cache[i - 4] + (seq_t)2 * cache[i - 5]
                 - cache[i - 9] - cache[i - 10];
    }
    last = n;
    return cache[n];
}
