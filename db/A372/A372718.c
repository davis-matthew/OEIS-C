/*
 * A372718  Triangular numbers that are 2 mod 4, halved.
 * https://oeis.org/A372718
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A372718: Triangular numbers that are 2 mod 4, halved. 
 * a(n) satisfies the linear recurrence
 *   a(n) = a(n-1) + 2*a(n-2) - 2*a(n-3) - a(n-4) + a(n-5)   (n > 5)
 * with initial values
 *   a(1)=3, a(2)=5, a(3)=33, a(4)=39, a(5)=95.
 * This implementation memoises results in a dynamically resized
 * array; the array is grown as needed.  The code compiles in
 * both C11 and C++17.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0)   /* offset starts at 1 – the sequence is undefined for n=0 */
        return 0;

    /* static memoisation store */
    static seq_t *cache   = NULL;
    static size_t  cap    = 0;
    static idx_t   up_to  = 0;   /* highest index computed so far */

    /* initialise the cache the first time */
    if (!cache) {
        cap   = 16;                       /* initial capacity */
        cache = (seq_t *)calloc(cap, sizeof(seq_t));
        if (!cache) exit(EXIT_FAILURE);    /* allocation failed */

        /* set the first five values */
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)5;
        cache[3] = (seq_t)33;
        cache[4] = (seq_t)39;
        cache[5] = (seq_t)95;
        up_to   = 5;
    }

    /* grow the cache if necessary */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = cap;
        while ((size_t)n + 1 > new_cap)
            new_cap <<= 1;

        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);    /* allocation failed */
        cache = tmp;
        cap   = new_cap;
    }

    /* compute terms up to n if they are not yet present */
    for (size_t i = (size_t)up_to + 1; i <= (size_t)n; ++i) {
        seq_t v;
        if (i <= 5) {
            /* the initial five terms are already set */
            continue;
        }
        /* recurrence: a(n) = a(n-1) + 2*a(n-2) - 2*a(n-3) - a(n-4) + a(n-5) */
        v = cache[i - 1];
        v += (seq_t)2 * cache[i - 2];
        v -= (seq_t)2 * cache[i - 3];
        v -= cache[i - 4];
        v += cache[i - 5];
        cache[i] = v;
    }

    up_to = (idx_t)n;
    return cache[n];
}
