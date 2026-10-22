/*
 * A308158  Sum of the smallest side lengths of all integer-sided isosceles
 *          triangles with perimeter n.
 * https://oeis.org/A308158
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-3) + 2*a(n-4) - 2*a(n-7) - a(n-8) + a(n-11)
   for n>11.
   Initial values:
     a(0) = 0, a(1)=0, a(2)=0, a(3)=1, a(4)=0, a(5)=1,
     a(6)=2, a(7)=3, a(8)=2, a(9)=4, a(10)=5, a(11)=7  */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* static cache holding already computed terms */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;

    /* initialise cache with known values up to n = 11 */
    if (!cache) {
        cache_size = 12; /* indices 0..11 */
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) return (seq_t)0;   /* out of memory, return 0 */
        /* set all to 0 first */
        for (size_t i = 0; i < cache_size; ++i) cache[i] = (seq_t)0;

        /* explicit initial values */
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)0;
        cache[3] = (seq_t)1;
        cache[4] = (seq_t)0;
        cache[5] = (seq_t)1;
        cache[6] = (seq_t)2;
        cache[7] = (seq_t)3;
        cache[8] = (seq_t)2;
        cache[9] = (seq_t)4;
        cache[10] = (seq_t)5;
        cache[11] = (seq_t)7;
    }

    /* trivial case: n < 1 */
    if (n < 1) return (seq_t)0;

    /* extend cache if needed */
    if ((size_t)n >= cache_size) {
        size_t new_size = (size_t)n + 1;  /* include index n */
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return (seq_t)0;  /* reallocation failed */
        cache = tmp;

        /* fill newly required entries */
        for (size_t i = cache_size; i < new_size; ++i) {
            /* i corresponds to index i, i >= 12 here */
            long long v =   (long long)cache[i-3]
                          + 2LL * (long long)cache[i-4]
                          - 2LL * (long long)cache[i-7]
                          -     (long long)cache[i-8]
                          +     (long long)cache[i-11];
            cache[i] = (seq_t)v;
        }
        cache_size = new_size;
    }

    return cache[n];
}
