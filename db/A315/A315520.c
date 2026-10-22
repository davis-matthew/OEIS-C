/*
 * A315520  Coordination sequence Gal.4.140.3 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315520
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) - a(n-2) - a(n-4) + 2*a(n-5) - a(n-6) for n > 6,
 *            with a(0)=1, a(1)=6, a(2)=11, a(3)=17, a(4)=22, a(5)=27, a(6)=33
 * Range:     u8 n <= 46, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache: largest index computed so far */
    static seq_t *cache = NULL;
    static size_t max_n  = 0;

    /* initial values for the first 7 terms */
    static const seq_t base[7] = {1, 6, 11, 17, 22, 27, 33};

    /* trivial access for very small n */
    if (n < 7)
        return base[n];

    /* first call: allocate storage for the 7 initial terms */
    if (cache == NULL) {
        cache = (seq_t *)malloc(7 * sizeof(seq_t));
        if (!cache)
            abort();                  /* allocation failure */
        for (size_t i = 0; i < 7; ++i)
            cache[i] = base[i];
        max_n = 6;
    }

    /* grow the cache if needed */
    if ((size_t)n > max_n) {
        /* allocate space up to n (inclusive) */
        seq_t *tmp = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!tmp)
            abort();                  /* allocation failure */
        cache = tmp;
        /* compute missing terms using the recurrence */
        for (size_t i = max_n + 1; i <= (size_t)n; ++i) {
            cache[i] = 2 * cache[i - 1]
                       - cache[i - 2]
                       - cache[i - 4]
                       + 2 * cache[i - 5]
                       - cache[i - 6];
        }
        max_n = n;
    }

    return cache[(size_t)n];
}
