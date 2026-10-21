/*
 * A298542  Number of nX3 0..1 arrays with every element equal to 1, 2, 3, 4
 *          or 7 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A298542
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-1) + 3*a(n-2) + 6*a(n-3) - 15*a(n-4) - 11*a(n-5) -
 *            5*a(n-6) - 46*a(n-7) - 2*a(n-8) - 38*a(n-9) - 16*a(n-10) -
 *            8*a(n-11) - 8*a(n-12), initial values (n = 1 .. 13): a(1) = 1,
 *            a(2) = 17, a(3) = 109, a(4) = 588, a(5) = 3276, a(6) = 18500,
 *            a(7) = 104034, a(8) = 585134, a(9) = 3291766, a(1...
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 25, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function uses a memoised recurrence; values are cached
 * up to the largest n requested.  It works for any seq_t
 * and any idx_t (uint64_t or other). */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;

    /* static cache of computed values */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;          /* allocated number of slots (≥ n+1) */
    static idx_t current_max = 0;         /* highest n for which cache[n] is defined */

    /* ensure sufficient capacity */
    idx_t needed = n + 1;                 /* include unused index 0 */
    if (cache_len < (size_t)needed) {
        size_t new_len = cache_len ? cache_len * 2 : 16;
        if (new_len < (size_t)needed) new_len = needed;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        cache = tmp;
        cache_len = new_len;
    }

    /* initialise first 13 terms once */
    if (current_max == 0) {
        cache[0] = (seq_t)0;
        cache[1]  = (seq_t)1;
        cache[2]  = (seq_t)17;
        cache[3]  = (seq_t)109;
        cache[4]  = (seq_t)588;
        cache[5]  = (seq_t)3276;
        cache[6]  = (seq_t)18500;
        cache[7]  = (seq_t)104034;
        cache[8]  = (seq_t)585134;
        cache[9]  = (seq_t)3291766;
        cache[10] = (seq_t)18516878;
        cache[11] = (seq_t)104162234;
        cache[12] = (seq_t)585942186;
        cache[13] = (seq_t)3296084046;
        current_max = 13;
    }

    /* compute any missing terms up to n */
    while (current_max < n) {
        idx_t i = current_max + 1;
        seq_t term = (seq_t)5 * cache[i-1]
                   + (seq_t)3 * cache[i-2]
                   + (seq_t)6 * cache[i-3]
                   - (seq_t)15 * cache[i-4]
                   - (seq_t)11 * cache[i-5]
                   - (seq_t)5  * cache[i-6]
                   - (seq_t)46 * cache[i-7]
                   - (seq_t)2  * cache[i-8]
                   - (seq_t)38 * cache[i-9]
                   - (seq_t)16 * cache[i-10]
                   - (seq_t)8  * cache[i-11]
                   - (seq_t)8  * cache[i-12];
        cache[i] = term;
        current_max = i;
    }

    return cache[n];
}
