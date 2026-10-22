/*
 * A310509  Coordination sequence Gal.5.142.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A310509
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: 
   a(0)=1, a(1)=4, a(2)=10, a(3)=16, a(4)=21, a(5)=25,
   a(6)=30, a(7)=36, a(8)=42, a(9)=46, a(10)=50,
   a(n)=a(n-1)+a(n-9)-a(n-10) for n>10 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t capacity = 0;
    static size_t len = 0;        /* number of terms already computed */

    /* ensure array capacity */
    if ((size_t)n + 1 > capacity) {
        size_t new_cap = (size_t)n + 1;
        if (new_cap < 16) new_cap = 16;
        else if (new_cap < capacity * 2) new_cap = capacity * 2;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        capacity = new_cap;
    }

    /* initialise first 11 terms lazily */
    if (len == 0) {
        cache[0]  = (seq_t)1;
        cache[1]  = (seq_t)4;
        cache[2]  = (seq_t)10;
        cache[3]  = (seq_t)16;
        cache[4]  = (seq_t)21;
        cache[5]  = (seq_t)25;
        cache[6]  = (seq_t)30;
        cache[7]  = (seq_t)36;
        cache[8]  = (seq_t)42;
        cache[9]  = (seq_t)46;
        cache[10] = (seq_t)50;
        len = 11;   /* next index to compute */
    }

    /* fill cache up to requested n */
    while (len <= (size_t)n) {
        cache[len] = cache[len-1] + cache[len-9] - cache[len-10];
        ++len;
    }

    return cache[(size_t)n];
}
