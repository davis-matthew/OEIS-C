/*
 * A361610  a(n) = 5^n*(n+1)*(4*n^2+14*n+3)/3.
 * https://oeis.org/A361610
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 9, u64 n <= 21, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 20*a(n-1)-150*a(n-2)+500*a(n-3)-625*a(n-4);
   with initial values:
   a(0)=1, a(1)=70, a(2)=1175, a(3)=13500 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* cached terms */
    static idx_t cached_upto = 0; /* highest index computed */
    static size_t cap = 0;        /* capacity of cache */

    if (n == 0) {
        return (seq_t)1;
    }

    /* Initialise cache with the first four terms on first call */
    if (cache == NULL) {
        const size_t init_cap = 8;
        cache = (seq_t*)malloc(sizeof(seq_t) * init_cap);
        if (!cache) return (seq_t)0;          /* allocation failed */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)70;
        cache[2] = (seq_t)1175;
        cache[3] = (seq_t)13500;
        cap      = init_cap;
        cached_upto = 3;
    }

    /* If the requested n is already cached, return it */
    if (n <= cached_upto) {
        return cache[n];
    }

    /* Extend cache as needed */
    while (cached_upto < n) {
        idx_t i = cached_upto + 1;

        /* Compute next term using the recurrence */
        seq_t term  = 20 * cache[i-1];
        term        = term - 150 * cache[i-2];
        term        = term + 500 * cache[i-3];
        term        = term - 625 * cache[i-4];

        /* Ensure there is enough space in the array */
        if ((size_t)i + 1 > cap) {
            size_t new_cap = cap * 2;
            if (new_cap == 0) new_cap = 8;
            while ((size_t)i + 1 > new_cap) new_cap *= 2;
            seq_t *tmp = (seq_t*)realloc(cache, sizeof(seq_t) * new_cap);
            if (!tmp) return (seq_t)0;  /* allocation failed */
            cache = tmp;
            cap = new_cap;
        }

        cache[i] = term;
        cached_upto = i;
    }

    return cache[n];
}
