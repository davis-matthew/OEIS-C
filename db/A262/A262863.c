/*
 * A262863  Binary representation of the middle column of the "Rule 147"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A262863
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 11*a(n-1) - 11*a(n-2) + 11*a(n-3) - 10*a(n-4) for n>3
 *            a(0) = 1, a(1) = 10, a(2) = 100, a(3) = 1001
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for arbitrary order calls */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;      /* number of elements already stored */

    if (cache_size <= n) {
        idx_t new_size = n + 1;
        seq_t *new_cache = NULL;

#if defined(__cplusplus)
        /* C++: use new/ delete to construct/ destroy objects */
        new_cache = new seq_t[new_size];
        for (idx_t i = 0; i < cache_size; ++i)
            new_cache[i] = cache[i];
        delete[] cache;
#else
        /* C: realloc is sufficient for built‑in integer types */
        new_cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_size);
        if (!new_cache)
            abort();   /* allocation failure */
#endif
        cache = new_cache;

        /* compute new terms if needed */
        for (idx_t i = cache_size; i < new_size; ++i) {
            if (i == 0)
                cache[0] = (seq_t)1;
            else if (i == 1)
                cache[1] = (seq_t)10;
            else if (i == 2)
                cache[2] = (seq_t)100;
            else if (i == 3)
                cache[3] = (seq_t)1001;
            else {
                seq_t t = (seq_t)11 * cache[i-1]
                        - (seq_t)11 * cache[i-2]
                        + (seq_t)11 * cache[i-3]
                        - (seq_t)10 * cache[i-4];
                cache[i] = t;
            }
        }
        cache_size = new_size;
    }

    return cache[n];
}
