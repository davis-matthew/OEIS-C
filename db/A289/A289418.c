/*
 * A289418  Number of Dyck paths of semilength n and height exactly 5.
 * https://oeis.org/A289418
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9*a(n-1) - 29*a(n-2) + 40*a(n-3) - 22*a(n-4) + 3*a(n-5)
 *            for n >= 6, a(5)=1
 * Range:     u8 n <= 8, u16 n <= 12, u32 n <= 21, u64 n <= 39, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for memoisation.
       cache[i] holds a(i).  Indices 0‑4 are unused. */
    static seq_t *cache   = NULL;   /* capacity at least n+1 */
    static size_t  cap    = 0;      /* current capacity        */
    static idx_t   computed = 4;    /* largest index with known value */

    if (n < 5)
        return (seq_t)0;            /* behaviour below the offset is irrelevant */

    /* Expand cache if required */
    if ((size_t)(n + 1) > cap) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            abort();              /* allocation failure */

        cache = tmp;

        /* initialise newly allocated part to zero */
        for (size_t j = cap; j < new_cap; ++j)
            cache[j] = 0;

        cap = new_cap;
    }

    /* Initialise base value a(5) if not already done */
    if (computed < 5) {
        cache[5] = (seq_t)1;
        computed = 5;
    }

    /* Compute missing values up to n */
    for (idx_t i = computed + 1; i <= n; ++i) {
        seq_t sum = cache[i - 1] * 9;
        sum = sum - (cache[i - 2] * 29);
        sum = sum + (cache[i - 3] * 40);
        sum = sum - (cache[i - 4] * 22);
        sum = sum + (cache[i - 5] * 3);
        cache[i] = sum;
    }
    computed = n;

    return cache[n];
}
