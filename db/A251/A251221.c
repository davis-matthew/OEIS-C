/*
 * A251221  Number of (n+1) X (1+1) 0..1 arrays with no 2 X 2 subblock having
 *          the minimum of its diagonal elements greater than the absolute
 *          difference of its antidiagonal elements.
 * https://oeis.org/A251221
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=14, a(2)=49, a(3)=171, a(n)=3*a(n-1)+2*a(n-2)-a(n-3) for
 *            n>=4
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 16, u64 n <= 34, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS sequence A251221.
 * Requires seq_t and idx_t types to be predefined (see build system).
 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* static cache to allow out‑of‑order calls */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of terms currently stored */

    if ((size_t)n <= cache_len)
        return cache[n - 1];

    /* grow cache to accommodate n terms */
    size_t new_len = (size_t)n;
    cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));

    /* compute missing terms */
    for (idx_t i = (idx_t)(cache_len + 1); i <= n; ++i) {
        if (i == 1)
            cache[i - 1] = 14;
        else if (i == 2)
            cache[i - 1] = 49;
        else if (i == 3)
            cache[i - 1] = 171;
        else
            /* recurrence: a(n)=3*a(n-1)+2*a(n-2)-a(n-3) */
            cache[i - 1] = 3 * cache[i - 2] + 2 * cache[i - 3] - cache[i - 4];
    }

    cache_len = new_len;
    return cache[n - 1];
}
