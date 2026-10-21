/*
 * A296033  Number of n X 2 0..1 arrays with each 1 adjacent to 2 or 4
 *          king-move neighboring 1s.
 * https://oeis.org/A296033
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 6*a(n-3) + 4*a(n-5), a(1)=1, a(2)=5, a(3)=11,
 *            a(4)=21, a(5)=55
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 26, u64 n <= 53, u128 n <= 107,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Memoisation cache: 1‑based indices stored as 0‑based array */
    static seq_t *cache = NULL;
    static size_t  capacity = 0;      /* allocated size of cache (elements) */
    static idx_t   cur      = 0;      /* highest index computed (1‑based) */

    if (n == 0) return (seq_t)0;      /* not required, but harmless */

    /* Resize cache if necessary */
    if (n > capacity) {
        size_t newcap = capacity ?: 8;          /* start with 8 if empty */
        while (newcap < n) newcap <<= 1;        /* grow by factor 2 */

        /* Allocate new array */
        seq_t *newbuf;
#ifdef __cplusplus
        newbuf = new seq_t[newcap];
#else
        newbuf = calloc(newcap, sizeof(seq_t));
#endif
        /* Copy existing values */
        for (idx_t i = 0; i < cur; ++i)
            newbuf[i] = cache[i];

        /* Free old buffer */
        if (cache) {
#ifdef __cplusplus
            delete[] cache;
#else
            free(cache);
#endif
        }
        cache    = newbuf;
        capacity = newcap;
    }

    /* Compute missing terms */
    for (idx_t i = cur + 1; i <= n; ++i) {  /* i is 1‑based */
        seq_t val;
        if (i == 1)          val = (seq_t)1;
        else if (i == 2)     val = (seq_t)5;
        else if (i == 3)     val = (seq_t)11;
        else if (i == 4)     val = (seq_t)21;
        else if (i == 5)     val = (seq_t)55;
        else
            val = cache[i-2] + (seq_t)6 * cache[i-4] + (seq_t)4 * cache[i-6];
        cache[i-1] = val;
    }
    cur = n;

    return cache[n-1];
}
