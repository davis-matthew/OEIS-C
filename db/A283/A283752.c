/*
 * A283752  Decimal representation of the x-axis, from the left edge to the
 *          origin, of the n-th stage of growth of the two-dimensional
 *          cellular automaton defined by "Rule 758", based on the 5-celled
 *          von Neumann neighborhood.
 * https://oeis.org/A283752
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=3, a(2)=5, a(3)=13. For n ≥ 4: a(n) = 16 * a(n-4) +
 *            b(n), where b(n) = 1,3,5,13,5,5,13,-3,45 for n=0…8 and b(n)=45
 *            for n ≥ 8. The recurrence reproduces the OEIS sequence A283752
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 126,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache = NULL;
    static size_t cap = 0;        /* allocated size of cache      */
    static idx_t  last = 0;       /* highest index computed so far */

    /* initialise the cache with the first 4 known terms */
    if (cache == NULL) {
        cap = 16;                              /* start with some room   */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;    /* OOM: unlikely in tests */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)5;
        cache[3] = (seq_t)13;
        last = 3;
    }

    /* grow the cache if we need more space */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = (cap > 0) ? cap : 16;
        while (new_cap <= (size_t)n) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;      /* OOM: unlikely in tests */
        cache = tmp;
        cap = new_cap;
    }

    /* if the requested term is already prepared, return it */
    if (n <= last) return cache[n];

    /* values of the auxiliary sequence b(n) for 0…8 */
    static const int b_vals[9] = {1, 3, 5, 13, 5, 5, 13, -3, 45};

    /* compute successive terms until we reach n */
    for (idx_t i = last + 1; i <= n; ++i) {
        seq_t val = cache[i - 4] * (seq_t)16;   /* 16 * a(i-4) */
        int   bi  = (i <= 8) ? b_vals[(int)i] : 45;
        if (bi >= 0)
            val += (seq_t)bi;
        else
            val -= (seq_t)(-bi);                /* subtract the absolute value */
        cache[i] = val;
    }
    last = n;
    return cache[n];
}
