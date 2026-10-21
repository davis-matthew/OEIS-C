/*
 * A252258  Number of (n+2) X (2+2) 0..3 arrays with every 3 X 3 subblock row
 *          and column sum not equal to 0 2 3 6 or 7 and every 3 X 3 diagonal
 *          and antidiagonal sum equal to 0 2 3 6 or 7.
 * https://oeis.org/A252258
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 6*a(n-3) - 6*a(n-4) - 8*a(n-6) + 8*a(n-7), for n
 *            > 12 with initial values: a(1)=1331, a(2)=1016, a(3)=562,
 *            a(4)=512, a(5)=570, a(6)=782, a(7)=1178, a(8)=1670, a(9)=2474,
 *            a(10)=3890, a(11)=5642, a(12)=8594
 * Range:     u8 none, u16 n <= 16, u32 n <= 40, u64 n <= 88, u128 n <= 184,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* static cache of already computed terms  (a(1) at cache[0]) */
    static seq_t *cache = NULL;
    static idx_t cap = 0;          /* allocated size of cache */
    static idx_t max_n_computed = 0; /* largest n for which cache[n-1] is valid */

    /* initial seven constant values - use cast for proper seq_t type */
    static const seq_t init[12] = {
        (seq_t)1331, (seq_t)1016, (seq_t)562, (seq_t)512,
        (seq_t)570, (seq_t)782, (seq_t)1178, (seq_t)1670,
        (seq_t)2474, (seq_t)3890, (seq_t)5642, (seq_t)8594
    };

    /* first time: allocate cache and load initial terms (up to 12) */
    if (!cache) {
        cap = 16;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) { /* malloc failure -> return 0 as safe fallback */
            return (seq_t)0;
        }
        memcpy(cache, init, 12 * sizeof(seq_t));
        memset(cache + 12, 0, (cap - 12) * sizeof(seq_t));
        max_n_computed = 12;
    }

    /* if requested n <= max_n_computed, we are done */
    if (n <= max_n_computed) {
        return cache[n - 1];
    }

    /* ensure capacity for all needed terms up to n */
    if ((idx_t)cap < n) {
        idx_t new_cap = cap;
        while ((idx_t)new_cap < n) {
            new_cap <<= 1;               /* double until enough */
        }
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) {                /* realloc failure -> return 0 */
            free(cache);
            cache = NULL;
            cap = 0;
            return (seq_t)0;
        }
        cache = new_cache;
        /* initialize newly allocated part to zero */
        memset(cache + cap, 0, (new_cap - cap) * sizeof(seq_t));
        cap = new_cap;
    }

    /* compute terms from max_n_computed+1 up to n */
    for (idx_t k = max_n_computed + 1; k <= n; ++k) {
        /* recurrence for k > 12 */
        seq_t val = cache[k - 2];                                   /* a(k-1)  */
        val += ((seq_t)6) * cache[k - 4];                           /* + 6*a(k-3) */
        val -= ((seq_t)6) * cache[k - 5];                           /* - 6*a(k-4) */
        val -= ((seq_t)8) * cache[k - 7];                           /* - 8*a(k-6) */
        val += ((seq_t)8) * cache[k - 8];                           /* + 8*a(k-7) */
        cache[k - 1] = val;
    }
    max_n_computed = n;
    return cache[n - 1];
}
