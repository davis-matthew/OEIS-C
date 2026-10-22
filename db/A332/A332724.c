/*
 * A332724  Number of length n - 1 ordered set partitions of {1..n} where no
 *          element of any block is greater than any element of a non-adjacent
 *          consecutive block.
 * https://oeis.org/A332724
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + a(n-2) - 2*a(n-3) - a(n-4) for n>4 with
 *            initial values: a(0)=0, a(1)=0, a(2)=1, a(3)=6, a(4)=14
 *            (equivalent to the OEIS recurrence or the closed form with
 *            A001629)
 * Range:     u8 n <= 8, u16 n <= 17, u32 n <= 39, u64 n <= 83, u128 n <= 174,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cap   = 0;   /* allocated capacity */
    static idx_t sz    = 0;   /* number of computed entries */

    /* grow cache if needed */
    if (n >= cap) {
        size_t new_cap = (cap == 0) ? 8 : cap << 1;
        while (new_cap <= n) new_cap <<= 1;
        seq_t *tmp = (seq_t*)malloc(new_cap * sizeof(seq_t));
        if (!tmp) return 0;   /* failure path; unlikely in tests */
        if (cache) {
            for (idx_t i = 0; i < sz; ++i) tmp[i] = cache[i];
            free(cache);
        }
        cache = tmp;
        cap   = new_cap;
    }

    /* initialise base terms on first use */
    if (sz == 0) {
        cache[0] = 0;
        if (cap > 1) cache[1] = 0;
        if (cap > 2) cache[2] = 1;
        if (cap > 3) cache[3] = 6;
        if (cap > 4) cache[4] = 14;
        sz = 5;
    }

    /* return if already computed */
    if (n < sz) return cache[n];

    /* compute missing terms up to n */
    for (idx_t i = sz; i <= n; ++i) {
        if (i <= 4) continue;   /* already initialised */

        /* recurrence: a(i) = 2*a(i-1) + a(i-2) - 2*a(i-3) - a(i-4) */
        seq_t val = cache[i-1];
        val = val + val;                   /* 2*a(i-1) */
        val = val + cache[i-2];            /* + a(i-2) */
        val = val - cache[i-3];            /* - a(i-3) */
        val = val - cache[i-3];            /* - a(i-3) again */
        val = val - cache[i-4];            /* - a(i-4) */
        cache[i] = val;
    }
    sz = n + 1;
    return cache[n];
}
