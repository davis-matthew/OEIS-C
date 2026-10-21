/*
 * A255075  Number of (n+2) X (1+2) 0..1 arrays with no 3 x 3 subblock
 *          diagonal sum one and no antidiagonal sum one.
 * https://oeis.org/A255075
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-1) - 25*a(n-3) + 25*a(n-4) for n>5 with initial
 *            values: a(1)=208, a(2)=676, a(3)=2444, a(4)=8836, a(5)=31960
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 14, u64 n <= 31, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* memoisation cache */
    static seq_t *cache = NULL;     /* array of terms, indices 0..max_comp */
    static size_t cap   = 0;        /* current allocated size of cache */
    static idx_t max_comp = 0;     /* largest n already computed, >=0 */

    if (n == 0)                      /* position before the offset */
        return (seq_t)0;

    /* enlarge cache if required */
    if ((size_t)n >= cap) {
        size_t newcap = cap ? cap * 2 : (size_t)n + 1;
        if (newcap < (size_t)n + 1)
            newcap = (size_t)n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp)               /* allocation failure – return 0 */
            return (seq_t)0;
        cache = tmp;
        cap   = newcap;
    }

    /* already have the value? */
    if (max_comp >= n) {
        return cache[n];
    }

    /* base values (index 0 unused, kept as 0) */
    static const seq_t base[6] = {
        (seq_t)0,     /* a(0) not used */
        (seq_t)208,   /* a(1) */
        (seq_t)676,   /* a(2) */
        (seq_t)2444,  /* a(3) */
        (seq_t)8836,  /* a(4) */
        (seq_t)31960  /* a(5) */
    };

    /* compute missing terms up to n */
    for (idx_t i = max_comp + 1; i <= n; ++i) {
        if (i <= 5) {
            cache[i] = base[i];
        } else {        /* recurrence for n>5 */
            seq_t t1 = (seq_t)5   * cache[i-1];
            seq_t t2 = (seq_t)25  * cache[i-3];
            seq_t t3 = (seq_t)25  * cache[i-4];
            cache[i] = t1 - t2 + t3;
        }
    }

    max_comp = n;
    return cache[n];
}
