/*
 * A251751  9-step Fibonacci sequence starting with 0,0,1,0,0,0,0,0,0.
 * https://oeis.org/A251751
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n+9) = a(n)+a(n+1)+...+a(n+8) Initial values: a(0)=0, a(1)=0,
 *            a(2)=1, a(3)=0, a(4)=0, a(5)=0, a(6)=0, a(7)=0, a(8)=0
 * Range:     u8 n <= 17, u16 n <= 25, u32 n <= 41, u64 n <= 73, u128 n <= 137,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>  /* malloc, realloc */
#include <stddef.h>  /* size_t */

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;      /* dynamic array of computed terms */
    static size_t capacity = 0;      /* allocated array size */
    static idx_t  count = 0;         /* first index not yet computed */

    /* Initialise first 9 terms once */
    if (count == 0)
    {
        size_t init_cap = 9;              /* indices 0..8 */
        cache = (seq_t*)malloc(init_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;     /* allocation failure */

        /* set the initial values */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)0;
        cache[4] = (seq_t)0;
        cache[5] = (seq_t)0;
        cache[6] = (seq_t)0;
        cache[7] = (seq_t)0;
        cache[8] = (seq_t)0;

        capacity = init_cap;
        count    = 9;                     /* indices 0..8 are set */
    }

    /* Extend cache if needed */
    if (n >= count)
    {
        size_t new_cap = (size_t)(n + 1);
        seq_t *new_cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) return (seq_t)0; /* allocation failure */

        cache = new_cache;
        capacity = new_cap;

        /* Compute missing terms up to n */
        for (idx_t i = count; i <= n; ++i)
        {
            seq_t sum = 0;
            /* sum previous 9 terms: cache[i-9] .. cache[i-1] */
            for (size_t k = 0; k < 9; ++k)
                sum += cache[i - 9 + k];
            cache[i] = sum;
        }
        count = n + 1;
    }

    return cache[n];
}
