/*
 * A252379  Number of (n+2) X (3+2) 0..3 arrays with every 3 X 3 subblock row
 *          and diagonal sum equal to 0 3 5 6 or 7 and every 3 X 3 column and
 *          antidiagonal sum not equal to 0 3 5 6 or 7.
 * https://oeis.org/A252379
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-3) - 4*a(n-6) + a(n-9) (for n > 12); initial
 *            values: a(1)=1359, a(2)=647, a(3)=632, a(4)=616, a(5)=799,
 *            a(6)=984, a(7)=1114, a(8)=1541, a(9)=2007, a(10)=2417,
 *            a(11)=3483, a(12)=4686
 * Range:     u8 none, u16 n <= 20, u32 n <= 55, u64 n <= 124, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Static cache holds computed values a(1) .. a(len) (1‑based) */
    static seq_t *cache = NULL;
    static size_t len   = 0; /* number of stored terms */
    static size_t cap   = 0; /* allocated capacity */

    /* 12 known initial values */
    static const seq_t init[12] = {
        (seq_t)1359, (seq_t)647,  (seq_t)632,  (seq_t)616,
        (seq_t)799,  (seq_t)984,  (seq_t)1114, (seq_t)1541,
        (seq_t)2007, (seq_t)2417, (seq_t)3483, (seq_t)4686
    };

    if (n == 0)             /* out of range, but never occurs */
        return (seq_t)0;

    /* Return an initial value directly when requested */
    if (n <= 12)
        return init[(size_t)(n - 1)];

    /* Ensure the cache can hold up to n terms */
    size_t need = (size_t)n;
    if (need > cap) {
        seq_t *new_cache = (seq_t *)malloc(need * sizeof(seq_t));
        if (!new_cache)          /* allocation failure: best effort */
            return (seq_t)0;
        if (cache) {
            memcpy(new_cache, cache, len * sizeof(seq_t));
            free(cache);
        }
        cache = new_cache;
        cap   = need;
    }

    /* Store initial 12 terms into the cache if not already done */
    if (len < 12) {
        size_t i;
        for (i = len; i < 12; ++i)
            cache[i] = init[i];
        len = 12;
    }

    /* Compute missing terms up to a(n) using the recurrence */
    for (size_t i = len; i < need; ++i) {
        /* i is a 0‑based index for a(i+1); we need a(i-2)=a((i+1)-3),
           a(i-5)=a((i+1)-6), a(i-8)=a((i+1)-9)   */
        seq_t term = (seq_t)4 * cache[i - 3];
        term -= (seq_t)4 * cache[i - 6];
        term += cache[i - 9];
        cache[i] = term;
    }

    len = need;
    return cache[(size_t)(n - 1)];
}
