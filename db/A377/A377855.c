/*
 * A377855  Cogrowth sequence of the 16-element group C4:C4 = <S,T | S^4, T^4,
 *          STST^3>.
 * https://oeis.org/A377855
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 33, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence from the generating function
   a(0)=1, a(1)=0, a(2)=2, a(3)=6, a(4)=40
   for n≥5:  a(n)=2*a(n‑1)+4*a(n‑2)+8*a(n‑3)+32*a(n‑4) */
#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;
    static idx_t last_computed = 4;

    /* initialise cache with the first five terms */
    if (cache == NULL) {
        cache_cap = 5;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;          /* out of memory */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)2;
        cache[3] = (seq_t)6;
        cache[4] = (seq_t)40;
        last_computed = 4;
    }

    /* if the value is already known, return it */
    if (n <= last_computed) return cache[n];

    /* grow the storage if necessary */
    if (n >= cache_cap) {
        idx_t new_cap = cache_cap;
        while ((idx_t)n >= new_cap) new_cap <<= 1;   /* double until big enough */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;            /* out of memory */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute terms up to n using the recurrence */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        cache[i] =
            (seq_t)2  * cache[i-1] +
            (seq_t)4  * cache[i-2] +
            (seq_t)8  * cache[i-3] +
            (seq_t)32 * cache[i-4];
    }
    last_computed = n;
    return cache[n];
}
