/*
 * A316810  Number of nX3 0..1 arrays with every element unequal to 0, 1, 2,
 *          3, 4, 5 or 7 king-move adjacent elements, with upper left element
 *          zero.
 * https://oeis.org/A316810
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = { * a(1)=4, a(2)=32, a(3)=227, a(4)=1603, a(5)=11339,
 *            a(6)=80196, a(7)=567185 * for n≥8 :
 *            a(n)=6*a(n-1)+10*a(n-2)-7*a(n-3)-64*a(n-4)-51*a(n-5) * }
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache of computed terms */
    static seq_t *cache = NULL;
    static size_t cap = 0;      /* allocated size (index 0 unused) */
    static idx_t sz = 0;        /* largest computed index */

    /* initial values for n = 1 … 7 */
    static const seq_t init[7] = {
        (seq_t)4,
        (seq_t)32,
        (seq_t)227,
        (seq_t)1603,
        (seq_t)11339,
        (seq_t)80196,
        (seq_t)567185
    };

    if (n <= 7) {
        return init[n - 1];
    }

    /* Create cache on first use */
    if (cache == NULL) {
        cap = 32;                 /* start with some spare space */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        /* initialise the first 7 entries */
        for (idx_t i = 1; i <= 7; ++i) {
            cache[i] = init[i - 1];
        }
        sz = 7;
    }

    /* Expand cache if necessary */
    if ((idx_t)n >= cap) {
        size_t new_cap = cap * 2;
        while ((idx_t)new_cap <= n) {
            new_cap *= 2;
        }
        seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        for (idx_t i = 1; i <= sz; ++i) {
            new_cache[i] = cache[i];
        }
        free(cache);
        cache = new_cache;
        cap = new_cap;
    }

    /* Compute missing terms up to n using the recurrence */
    for (idx_t i = sz + 1; i <= n; ++i) {
        seq_t term = (seq_t)6 * cache[i - 1];
        term += (seq_t)10 * cache[i - 2];
        term -= (seq_t)7  * cache[i - 3];
        term -= (seq_t)64 * cache[i - 4];
        term -= (seq_t)51 * cache[i - 5];
        cache[i] = term;
    }
    sz = n;
    return cache[n];
}
