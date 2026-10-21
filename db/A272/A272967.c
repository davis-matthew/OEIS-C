/*
 * A272967  Number of nX2 0..1 arrays with exactly n+2-2 having value 1 and no
 *          three 1s forming an isosceles triangle.
 * https://oeis.org/A272967
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 30, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = a(n-1) + a(n-2) - a(n-3)  for n > 11
 * Initial values:
 *   a(1)=2,  a(2)=6,  a(3)=10,  a(4)=21,  a(5)=14,
 *   a(6)=28, a(7)=18, a(8)=29,  a(9)=22,  a(10)=32,
 *   a(11)=26
 *
 * The function memoises values in a static array, expanding it as needed.
 */
#include <stdlib.h>

seq_t a(idx_t n) {
    /* Static memoisation */
    static seq_t *cache = NULL;      /* indexed from 1 */
    static idx_t   cached_n = 0;     /* highest index computed */
    static size_t  capacity = 0;     /* allocated size of cache */

    /* initialise cache on first call */
    if (cache == NULL) {
        capacity = 12;               /* store a[1] .. a[11] plus one slot for 0 */
        cache = (seq_t *)malloc(capacity * sizeof(seq_t));
        if (!cache) return 0;       /* allocation failed: return 0 */

        /* initialise first 11 terms */
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)6;
        cache[3] = (seq_t)10;
        cache[4] = (seq_t)21;
        cache[5] = (seq_t)14;
        cache[6] = (seq_t)28;
        cache[7] = (seq_t)18;
        cache[8] = (seq_t)29;
        cache[9] = (seq_t)22;
        cache[10] = (seq_t)32;
        cache[11] = (seq_t)26;
        cached_n = 11;
    }

    /* guard against n < 1 (not specified, but reasonable) */
    if (n == 0) return 0;

    /* enlarge cache if required */
    if ((size_t)n + 1 > capacity) {
        size_t new_cap = (n + 1) * 2;  /* grow by factor 2 for future calls */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;           /* allocation failed */
        cache = tmp;
        capacity = new_cap;
    }

    /* compute missing values */
    for (idx_t i = cached_n + 1; i <= n; ++i) {
        if (i <= 11) {
            /* already initialized */
            continue;
        }
        cache[i] = cache[i-1] + cache[i-2] - cache[i-3];
    }
    cached_n = n;

    return cache[n];
}
