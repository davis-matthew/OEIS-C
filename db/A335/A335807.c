/*
 * A335807  Number of vertices in the n-th simplex graph of the complete graph
 *          on three vertices (K_3).
 * https://oeis.org/A335807
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 21, u64 n <= 44, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 4*a(n-1) - 4*a(n-2) + a(n-3)
   Initial values: a(0)=3, a(1)=8, a(2)=21, a(3)=54
   (This recurrence holds for n>3.) */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cap   = 0;   /* capacity of cache */
    static size_t used  = 0;   /* number of computed entries */

    /* initialise cache with room for at least 4 entries */
    if (cache == NULL) {
        cap = 4;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0; /* allocation failure */
        cache[0] = (seq_t)3;
        cache[1] = (seq_t)8;
        cache[2] = (seq_t)21;
        cache[3] = (seq_t)54;
        used = 4;
    }

    /* grow cache if required */
    if ((size_t)(n + 1) > cap) {
        size_t new_cap = cap * 2;
        if (new_cap < (size_t)(n + 1)) new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0; /* allocation failure */
        cache = tmp;
        cap = new_cap;
    }

    /* if value already computed, just return it */
    if ((size_t)n < used) return cache[n];

    /* compute values up to n using the recurrence */
    for (size_t i = used; i <= (size_t)n; ++i) {
        /* recurrence is valid for i >= 4 */
        seq_t t1 = (seq_t)4 * cache[i - 1];
        seq_t t2 = t1 - (seq_t)4 * cache[i - 2];
        cache[i] = t2 + cache[i - 3];
    }

    used = (size_t)n + 1;
    return cache[n];
}
