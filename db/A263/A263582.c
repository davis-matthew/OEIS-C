/*
 * A263582  Number of (n+1)X(1+1) arrays of permutations of 0..n*2+1 filled by
 *          rows with each element moved a city block distance of 1 or 2, and
 *          rows and columns in increasing lexicographic order.
 * https://oeis.org/A263582
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) + 2*a(n-3) + 4*a(n-4) - 8*a(n-5), n ≥ 6 initial
 *            values: a(1)=1, a(2)=4, a(3)=18, a(4)=70, a(5)=292
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;   /* undefined for n = 0 */

    /* static cache */
    static seq_t *cache    = NULL;   /* array indexed from 0; cache[0] unused */
    static idx_t  comp     = 0;      /* highest index that has been filled */
    static idx_t  alloc_sz = 0;      /* current allocation size (≥ comp+1) */

    /* initialise with the first five terms */
    if (comp < 5) {
        if (alloc_sz < 6) {
            seq_t *tmp = (seq_t *)realloc(cache, (size_t)6 * sizeof(seq_t));
            if (!tmp) abort();
            cache = tmp;
            alloc_sz = 6;
        }
        cache[0] = (seq_t)0;          /* unused */
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)4;
        cache[3] = (seq_t)18;
        cache[4] = (seq_t)70;
        cache[5] = (seq_t)292;
        comp = 5;
    }

    /* if requested value already computed, return it */
    if (n <= comp)
        return cache[n];

    /* enlarge the array if necessary */
    if (alloc_sz < n + 1) {
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)(n + 1) * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        alloc_sz = n + 1;
    }

    /* compute missing values */
    for (idx_t i = comp + 1; i <= n; ++i) {
        seq_t a1 = cache[i - 1];
        seq_t a3 = cache[i - 3];
        seq_t a4 = cache[i - 4];
        seq_t a5 = cache[i - 5];
        seq_t val = 4 * a1 + 2 * a3 + 4 * a4 - 8 * a5;
        cache[i] = val;
    }

    comp = n;
    return cache[n];
}
