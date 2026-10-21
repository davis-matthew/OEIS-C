/*
 * A284178  Decimal representation of the x-axis, from the origin to the right
 *          edge, of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 809", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A284178
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 126,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A284178: Decimal representation of the x‑axis, from the origin to
   the right edge, of the n‑th stage of growth of the two‑dimensional
   cellular automaton defined by "Rule 809".  
   Recurrence (for n ≥ 18):
        a(n) = 4*a(n-2) + a(n-8) - 4*a(n-10)
   Initial values (n = 0…17):
        1,0,4,0,20,0,84,16,292,64,1172,272,4644,1344,19092,5392,76324,21824
*/
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static idx_t sz   = 0;   /* number of stored terms */

    if (!cache) {
        /* Pre‑allocate enough space for the first 18 terms */
        cap = 64;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));

        /* initialise all entries to 0 */
        for (size_t i = 0; i < cap; ++i)
            cache[i] = 0;

        /* the first 18 terms (0 … 17) */
        const seq_t init[18] = {
            1, 0, 4, 0, 20, 0, 84, 16, 292, 64,
            1172, 272, 4644, 1344, 19092, 5392, 76324, 21824
        };
        for (idx_t i = 0; i < 18; ++i)
            cache[i] = init[i];

        sz = 18;   /* next index to compute */
    }

    if (n < sz)
        return cache[n];

    /* Ensure there is enough room for n+1 elements */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = cap ? cap * 2 : 64;
        while (new_cap < (size_t)n + 1)
            new_cap *= 2;

        seq_t *new_arr = (seq_t *)malloc(new_cap * sizeof(seq_t));
        for (size_t i = 0; i < cap; ++i)
            new_arr[i] = cache[i];
        for (size_t i = cap; i < new_cap; ++i)
            new_arr[i] = 0;
        free(cache);
        cache = new_arr;
        cap   = new_cap;
    }

    /* Compute missing terms using the recurrence (n >= 18) */
    for (idx_t i = sz; i <= n; ++i) {
        cache[i] = (seq_t)4 * cache[i-2] + cache[i-8] - (seq_t)4 * cache[i-10];
    }
    sz = n + 1;
    return cache[n];
}
