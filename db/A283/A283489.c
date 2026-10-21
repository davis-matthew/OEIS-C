/*
 * A283489  Number of nX3 0..1 arrays with no 1 equal to more than three of
 *          its horizontal, vertical and antidiagonal neighbors, with the
 *          exception of exactly one element.
 * https://oeis.org/A283489
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 10 a(n-1) + a(n-2) - 110 a(n-3) - 267 a(n-4) - 266 a(n-5)
 *            - 146 a(n-6) - 72 a(n-7) - 41 a(n-8) - 4 a(n-9) - 4 a(n-10) for
 *            n > 13 Initial values: a(1)=0, a(2)=2, a(3)=59, a(4)=576,
 *            a(5)=5444, a(6)=47712, a(7)=402588, a(8)=3304736, a(9)=26585976,
 *            a(10)=210604104, a(11)=1648084920, a(12)=1277...
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 22, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static const seq_t init_vals[14] = {
    [0] = 0,              /* unused, offset 1 */
    [1] = 0,
    [2] = 2,
    [3] = 59,
    [4] = 576,
    [5] = 5444,
    [6] = 47712,
    [7] = 402588,
    [8] = 3304736,
    [9] = 26585976,
    [10] = 210604104,
    [11] = 1648084920,
    [12] = 12770161232,
    [13] = 98143291572
};

seq_t a(idx_t n) {
    /* handle the offset */
    if (n <= 13) {
        return init_vals[n];
    }

    /* dynamic memoisation */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;          /* number of elements allocated (indices 0..cache_size-1) */
    static size_t computed = 13;           /* highest index already computed in cache */

    if (cache == NULL) {
        /* allocate space for indices 0..13 */
        cache_size = 14;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        for (size_t i = 0; i < cache_size; ++i) {
            if (i <= 13) cache[i] = init_vals[i];
            else         cache[i] = 0;
        }
    }

    /* enlarge cache if necessary */
    if ((size_t)n + 1 > cache_size) {
        size_t new_size = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) {
            /* out of memory; return 0 as a safe fallback */
            return (seq_t)0;
        }
        cache = tmp;
        /* initialise new elements */
        for (size_t i = cache_size; i < new_size; ++i) {
            cache[i] = 0;
        }
        cache_size = new_size;
    }

    /* compute new terms up to n */
    for (size_t i = computed + 1; i <= (size_t)n; ++i) {
        seq_t temp1 = (seq_t)10 * cache[i - 1] + cache[i - 2];
        seq_t temp2 = (seq_t)110 * cache[i - 3]
                    + (seq_t)267 * cache[i - 4]
                    + (seq_t)266 * cache[i - 5]
                    + (seq_t)146 * cache[i - 6]
                    + (seq_t) 72 * cache[i - 7]
                    + (seq_t) 41 * cache[i - 8]
                    + (seq_t)  4 * cache[i - 9]
                    + (seq_t)  4 * cache[i -10];
        cache[i] = temp1 - temp2;
    }
    computed = (size_t)n;

    return cache[n];
}
