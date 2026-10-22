/*
 * A391682  Number of integer compositions of n that are the first sums of
 *          more than one nonnegative sequence.
 * https://oeis.org/A391682
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 21, u32 n <= 44, u64 n <= 90, u128 n <= 183,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1, a(2)=2, a(3)=4, a(4)=7,
   a(n)=a(n-1)+3*a(n-2)-2*a(n-3)-2*a(n-4) for n>=5 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* values below the offset are undefined; return 0 */
    if (n < 1) return (seq_t)0;

    /* static cache: index 0 is unused, cache[i] holds a(i) for i>=1 */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;     /* highest index currently stored */

    idx_t prev_size = cache_size;    /* size before possible reallocation */

    /* allocate or grow cache to accommodate indices up to n */
    if (cache == NULL) {
        cache = (seq_t*)malloc((n + 1) * sizeof(seq_t));
        if (!cache) return (seq_t)0;   /* allocation failure */
        cache_size = n;
    } else if (n > cache_size) {
        seq_t *tmp = (seq_t*)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!tmp) return (seq_t)0;     /* allocation failure */
        cache = tmp;
        cache_size = n;
    }

    /* initialise base cases up to min(4, cache_size) */
    if (cache_size >= 1) cache[1] = (seq_t)1;
    if (cache_size >= 2) cache[2] = (seq_t)2;
    if (cache_size >= 3) cache[3] = (seq_t)4;
    if (cache_size >= 4) cache[4] = (seq_t)7;

    /* compute any missing entries via the recurrence */
    idx_t start = (prev_size >= 4) ? prev_size + 1 : 5;
    for (idx_t i = start; i <= n; ++i) {
        cache[i] = cache[i-1] + 3*cache[i-2] - 2*cache[i-3] - 2*cache[i-4];
    }

    return cache[n];
}
