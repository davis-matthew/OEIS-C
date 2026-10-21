/*
 * A250640  Number of length n+1 0..2 arrays with the sum of the maximum of
 *          each adjacent pair multiplied by some arrangement of +-1 equal to
 *          zero.
 * https://oeis.org/A250640
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1) - 3*a(n-2) - 42*a(n-3) + 71*a(n-4) + 56*a(n-5) -
 *            192*a(n-6) + 80*a(n-7) + 77*a(n-8) - 64*a(n-9) + 12*a(n-10) for
 *            n ≥ 11 Initial values: a(1)=1, a(2)=17, a(3)=23, a(4)=125,
 *            a(5)=280, a(6)=1061, a(7)=2870, a(8)=9495, a(9)=27507,
 *            a(10)=86149
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 40, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* size allocated (in elements) */
    static idx_t computed_max = 0; /* largest index whose value is known */

    if (n < 1) return 0;          /* undefined for n < 1 */

    /* Grow cache if necessary */
    if (n + 1 > cache_size) {
        idx_t new_size = n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache) exit(1); /* allocation failed */
        cache = new_cache;
        cache_size = new_size;
        /* Initialise new elements that were not previously allocated */
        if (computed_max < new_size - 1) {
            for (idx_t i = computed_max + 1; i < new_size; ++i) {
                /* For big‑integer types, the default constructor will be
                   called by malloc/realloc in the utility implementation. */
                cache[i] = 0;
            }
        }
    }

    /* Compute missing terms up to n */
    if (computed_max < n) {
        /* Use the first 10 known values */
        static seq_t base[11] = {
            0,          /* placeholder for index 0 */
            1,          /* a(1) */
            17,         /* a(2) */
            23,         /* a(3) */
            125,        /* a(4) */
            280,        /* a(5) */
            1061,       /* a(6) */
            2870,       /* a(7) */
            9495,       /* a(8) */
            27507,      /* a(9) */
            86149       /* a(10) */
        };

        for (idx_t i = computed_max + 1; i <= n; ++i) {
            if (i <= 10) {
                cache[i] = base[i];
            } else {
                seq_t val = 0;
                val += 6 * cache[i - 1];
                val -= 3 * cache[i - 2];
                val -= 42 * cache[i - 3];
                val += 71 * cache[i - 4];
                val += 56 * cache[i - 5];
                val -= 192 * cache[i - 6];
                val += 80 * cache[i - 7];
                val += 77 * cache[i - 8];
                val -= 64 * cache[i - 9];
                val += 12 * cache[i - 10];
                cache[i] = val;
            }
        }
        computed_max = n;
    }

    return cache[n];
}
