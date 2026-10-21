/*
 * A243383  Number of length n+3 0..2 arrays with no four elements in a row
 *          with pattern abba (with a!=b) and new values 0..2 introduced in
 *          0..2 order.
 * https://oeis.org/A243383
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) - 4*a(n-2) + 2*a(n-3) - a(n-4) (n ≥ 5) Initial
 *            values: a(1)=13, a(2)=35, a(3)=96, a(4)=265
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 20, u64 n <= 42, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* Cache for previously computed values. */
    static seq_t *cache = NULL;
    static idx_t alloc = 0;          /* size of the allocated array (max index that can be stored)   */
    static idx_t computed = 0;       /* highest index for which the sequence has actually been computed */

    if (n < 1)
        return 0;                     /* undefined for n < 1, but return 0 */

    /* Resize the cache if necessary. */
    if (cache == NULL) {
        alloc = (n > 4 ? n * 2 : 8);
        cache = (seq_t *)malloc((alloc + 1) * sizeof(seq_t));
        if (cache == NULL)
            return 0;
        computed = 0;
    } else if (n + 1 > alloc) {
        idx_t new_alloc = alloc * 2;
        if (new_alloc < n + 1) new_alloc = n + 1;
        seq_t *new_cache = (seq_t *)malloc((new_alloc + 1) * sizeof(seq_t));
        if (new_cache == NULL)
            return 0;
        memcpy(new_cache, cache, (alloc + 1) * sizeof(seq_t));
        free(cache);
        cache = new_cache;
        alloc = new_alloc;
    }

    /* Ensure base values are initialized. */
    if (computed == 0) {
        cache[1] = (seq_t)13;
        cache[2] = (seq_t)35;
        cache[3] = (seq_t)96;
        cache[4] = (seq_t)265;
        computed = 4;
    }

    /* Compute missing terms up to n. */
    for (idx_t i = computed + 1; i <= n; ++i) {
        if (i == 1) {
            cache[i] = (seq_t)13;
        } else if (i == 2) {
            cache[i] = (seq_t)35;
        } else if (i == 3) {
            cache[i] = (seq_t)96;
        } else if (i == 4) {
            cache[i] = (seq_t)265;
        } else {
            /* recurrence: a(i) = 4*a(i-1) - 4*a(i-2) + 2*a(i-3) - a(i-4) */
            seq_t val = ((seq_t)4 * cache[i-1]);
            val -= ((seq_t)4 * cache[i-2]);
            val += ((seq_t)2 * cache[i-3]);
            val -= cache[i-4];
            cache[i] = val;
        }
    }

    computed = n;
    return cache[n];
}
