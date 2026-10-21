/*
 * A274272  Number of partitions of 5^n into at most four parts.
 * https://oeis.org/A274272
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=6, a(2)=185, a(3)=15246, a(4)=1736385; * for n>4:
 *            a(n)=155*a(n-1)-3874*a(n-2)+15470*a(n-3)+3875*a(n-4)-15625*a(n-5)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 10, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* initial values for n = 0..4 */
    static const seq_t init[5] = { (seq_t)1, (seq_t)6, (seq_t)185,
                                   (seq_t)15246, (seq_t)1736385 };

    if (n < 5) return init[(size_t)n];

    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cap   = 0;

    /* initialise cache with first five terms if not yet done */
    if (!cache) {
        cap   = 5;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        for (size_t i = 0; i < 5; ++i) cache[i] = init[i];
    }

    size_t required = (size_t)n + 1;          /* need entries 0..n */
    if (required > cap) {
        /* remember old capacity before growing */
        size_t old_cap = cap;
        /* grow cache */
        seq_t *tmp = (seq_t *)realloc(cache, required * sizeof(seq_t));
        if (!tmp) return 0;                   /* allocation failed */
        cache = tmp;
        cap   = required;
        /* compute newly required terms */
        for (idx_t i = (idx_t)old_cap; i <= n; ++i) {
            seq_t pos = (seq_t)155 * cache[i-1]
                       + (seq_t)15470 * cache[i-3]
                       + (seq_t)3875 * cache[i-4];
            seq_t neg = (seq_t)3874 * cache[i-2]
                       + (seq_t)15625 * cache[i-5];
            cache[i] = pos - neg;
        }
    } else {
        /* cache already large enough – compute missing tail only */
        for (idx_t i = (idx_t)cap; i <= n; ++i) {
            seq_t pos = (seq_t)155 * cache[i-1]
                       + (seq_t)15470 * cache[i-3]
                       + (seq_t)3875 * cache[i-4];
            seq_t neg = (seq_t)3874 * cache[i-2]
                       + (seq_t)15625 * cache[i-5];
            cache[i] = pos - neg;
        }
    }

    return cache[(size_t)n];
}
