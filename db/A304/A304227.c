/*
 * A304227  Number of n X 5 0..1 arrays with every element unequal to 0, 1, 2
 *          or 7 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A304227
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 9*a(n-3) - 14*a(n-6) for n>10, with initial
 *            values a(1)=16, a(2)=8, a(3)=11, a(4)=20, a(5)=31, a(6)=89,
 *            a(7)=237, a(8)=494, a(9)=1162, a(10)=3043
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 26, u64 n <= 51, u128 n <= 102,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n) {
    /* static cache for memoized values */
    static seq_t *cache = NULL;
    static idx_t computed = 0;   /* highest n that has been computed */
    static size_t capacity = 0;  /* allocated size of cache (array indices 0..capacity-1) */

    if (n < 1) return 0; /* behaviour below the offset is unspecified */

    /* allocate and initialise cache on first call */
    if (cache == NULL) {
        capacity = 11;                /* indices 0..10 needed for initial data */
        cache = (seq_t *)calloc(capacity, sizeof(seq_t));
        if (!cache) return 0;         /* fallback on allocation failure */

        cache[1]  = (seq_t)16;
        cache[2]  = (seq_t)8;
        cache[3]  = (seq_t)11;
        cache[4]  = (seq_t)20;
        cache[5]  = (seq_t)31;
        cache[6]  = (seq_t)89;
        cache[7]  = (seq_t)237;
        cache[8]  = (seq_t)494;
        cache[9]  = (seq_t)1162;
        cache[10] = (seq_t)3043;
        computed = 10;
    }

    /* grow cache if the requested index exceeds current capacity */
    if ((size_t)(n + 1) > capacity) {
        capacity = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, capacity * sizeof(seq_t));
        if (!tmp) return 0;  /* fallback on allocation failure */
        cache = tmp;
    }

    /* compute missing terms up to n */
    if (n > computed) {
        for (idx_t k = computed + 1; k <= n; ++k) {
            cache[k] = cache[k-1]
                     + (seq_t)9 * cache[k-3]
                     - (seq_t)14 * cache[k-6];
        }
        computed = n;
    }

    return cache[n];
}
