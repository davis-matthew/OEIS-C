/*
 * A292299  Sum of values of vertices of type E at level n of the hyperbolic
 *          Pascal pyramid.
 * https://oeis.org/A292299
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 21, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A292299: Sum of values of vertices of type E at level n of 
   the hyperbolic Pascal pyramid.  
   a(0)=0, a(1)=0, a(2)=0, a(3)=0, a(4)=18, a(5)=312, a(6)=3798,
   a(n) = 18*a(n-1) - 99*a(n-2) + 226*a(n-3) - 224*a(n-4) + 92*a(n-5) - 12*a(n-6)  for n >= 7
*/
#include <stdlib.h>

static seq_t *cache   = NULL;   /* dynamic array of computed terms           */
static size_t cache_cap = 0;     /* current capacity of the cache             */
static idx_t  max_computed = 0;  /* largest n for which cache[n] is valid     */
static int    initialized   = 0; /* flag indicating whether base terms are set */

seq_t a(idx_t n)
{
    /* initialise base values on first use                                                     */
    if (!initialized) {
        /* allocate enough space for indices 0..6 inclusive, plus a spare slot for growth */
        size_t init_cap = 8;
        cache = (seq_t *)realloc(cache, init_cap * sizeof(seq_t));
        cache_cap = init_cap;
        /* set the first seven terms explicitly                                     */
        cache[0] = 0;
        cache[1] = 0;
        cache[2] = 0;
        cache[3] = 0;
        cache[4] = 18;
        cache[5] = 312;
        cache[6] = 3798;
        max_computed = 6;
        initialized = 1;
    }

    /* enlarge cache if necessary                                                             */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap > 0 ? cache_cap : 8;
        while (new_cap <= (size_t)n + 1)
            new_cap *= 2;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* compute missing terms up to n using the recurrence                                  */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        /* recurrence: a(i) = 18*a(i-1) - 99*a(i-2) + 226*a(i-3) - 224*a(i-4)
                         + 92*a(i-5) - 12*a(i-6)                                        */
        cache[(size_t)i] = 
              18 * cache[(size_t)(i-1)]
            - 99 * cache[(size_t)(i-2)]
            + 226 * cache[(size_t)(i-3)]
            - 224 * cache[(size_t)(i-4)]
            + 92  * cache[(size_t)(i-5)]
            - 12  * cache[(size_t)(i-6)];
    }
    max_computed = n > max_computed ? n : max_computed;

    return cache[(size_t)n];
}
