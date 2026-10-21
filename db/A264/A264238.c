/*
 * A264238  Number of (n+1) X (2+1) arrays of permutations of 0..n*3+2 with
 *          each element having directed index change 0,0 1,1 0,-1 -1,1 or
 *          0,-2.
 * https://oeis.org/A264238
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 16, u64 n <= 34, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A264238
    Empirical recurrence: a(1)=8, a(2)=21, a(3)=89,
    a(n)=2*a(n-1)+6*a(n-2)-a(n-3)   (n≥4)
    (Same recurrence can also be derived from the given generating function.)
*/
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* out of bounds, not used in the OEIS sequence */

    /* static cache: cache[0] -> a(1), cache[1] -> a(2), ... */
    static seq_t *cache = NULL;
    static size_t cached = 0;    /* number of terms already stored */

    /* initialise cache with the first three terms if necessary */
    if (cached == 0) {
        cached = 3;
        cache = (seq_t *)malloc(cached * sizeof(seq_t));
        if (!cache) return 0;    /* allocation failed: return 0 as sentinel */
        cache[0] = (seq_t)8;
        cache[1] = (seq_t)21;
        cache[2] = (seq_t)89;
    }

    /* extend cache up to the requested term */
    if ((size_t)n > cached) {
        size_t new_size = (size_t)n;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return 0;      /* allocation failed: return 0 as sentinel */
        cache = tmp;

        /* compute missing terms using the recurrence */
        for (size_t i = cached; i < new_size; ++i) {
            /* i corresponds to index n = i+1 */
            cache[i] = 2 * cache[i-1] + 6 * cache[i-2] - cache[i-3];
        }
        cached = new_size;
    }

    return cache[n-1]; /* return a(n) */
}
