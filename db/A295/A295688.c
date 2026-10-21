/*
 * A295688  a(n) = a(n-1) + a(n-3) + a(n-4), where a(0) = 2, a(1) = 1, a(2) =
 *          0, a(3) = 2.
 * https://oeis.org/A295688
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-3) + a(n-4); Initial values: a(0)=2, a(1)=1,
 *            a(2)=0, a(3)=2
 * Range:     u8 n <= 12, u16 n <= 24, u32 n <= 47, u64 n <= 93, u128 n <= 185,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle the base cases directly. */
    if (n == 0) return 2;
    if (n == 1) return 1;
    if (n == 2) return 0;
    if (n == 3) return 2;

    /* Lazy, dynamic memoization. */
    static seq_t *cache = NULL;   /* array of computed values      */
    static idx_t  size   = 0;     /* allocated size of cache       */
    static idx_t  computed = 0;   /* number of values actually known */

    /* Allocate initial storage (at least size 4 for the base terms). */
    if (cache == NULL) {
        size = 8;
        cache = (seq_t *)malloc(sizeof(seq_t) * size);
        cache[0] = 2;
        cache[1] = 1;
        cache[2] = 0;
        cache[3] = 2;
        computed = 4;
    }

    /* Resize if necessary to accommodate index n. */
    if (n >= size) {
        idx_t newsize = size ? size << 1 : 8;
        while (newsize <= n) newsize <<= 1;
        cache = (seq_t *)realloc(cache, sizeof(seq_t) * newsize);
        size = newsize;
    }

    /* Compute missing values up to n. */
    for (idx_t i = computed; i <= n; ++i) {
        cache[i] = cache[i - 1] + cache[i - 3] + cache[i - 4];
    }
    computed = n + 1;  /* we now know values up to n */

    return cache[n];
}
