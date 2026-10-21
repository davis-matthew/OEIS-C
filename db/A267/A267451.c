/*
 * A267451  Number of ON (black) cells in the n-th iteration of the "Rule 131"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A267451
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-3)+a(n-4)-a(n-7) for n>6 Initial values:
 *            a(0)=1,a(1)=1,a(2)=2,a(3)=2,a(4)=4,a(5)=3,a(6)=6
 * Range:     u8 n <= 339, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for already computed values */
    static seq_t *cache   = NULL;
    static size_t  cap    = 0;   /* capacity allocated in cache */
    static idx_t   last   = 0;   /* highest index computed so far */

    /* initialise cache with first 7 values when needed */
    if (cache == NULL) {
        cap = 7;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (cache == NULL) return 0; /* allocation failed */

        cache[0] = (seq_t)1; /* a(0) */
        cache[1] = (seq_t)1; /* a(1) */
        cache[2] = (seq_t)2; /* a(2) */
        cache[3] = (seq_t)2; /* a(3) */
        cache[4] = (seq_t)4; /* a(4) */
        cache[5] = (seq_t)3; /* a(5) */
        cache[6] = (seq_t)6; /* a(6) */
        last = 6;
    }

    /* enlarge cache if n is outside current capacity */
    if ((idx_t)cap <= n) {
        size_t new_cap = cap;
        while ((idx_t)new_cap <= n) new_cap <<= 1; /* double until big enough */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return 0; /* allocation failed */
        cache = tmp;
        cap = new_cap;
    }

    /* compute missing terms up to n using the recurrence */
    if (n > last) {
        for (idx_t i = last + 1; i <= n; ++i) {
            cache[i] =
                cache[i-3] + cache[i-4] - cache[i-7];
        }
        last = n;
    }

    return cache[n];
}
