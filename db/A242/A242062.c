/*
 * A242062  Expansion of x * (1 - x^12) / ((1 - x^3) * (1 - x^4) * (1 - x^7))
 *          in powers of x.
 * https://oeis.org/A242062
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1785, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A242062
   Formula: a(n) = a(n-7) + 1   (n >= 7)
   Initial values: a(0)=0, a(1)=1, a(2)=0, a(3)=0, a(4)=1, a(5)=1, a(6)=0
   Generates the sequence 0,1,0,0,1,1,0,1,2,1,1,2,2,1,2,3,2,2,3,3,2,3,4,3,...
*/
#include <stdlib.h>   /* malloc, realloc, abort */

/* initial values for indices 0..6 */
static const seq_t init_vals[7] = {0, 1, 0, 0, 1, 1, 0};

/* cache of computed terms, grows as needed */
static seq_t *cache = NULL;
static size_t cache_size = 0;

seq_t a(idx_t n)
{
    /* Ensure we have computed up to index n */
    if ((size_t)n + 1 > cache_size) {
        size_t new_size = (size_t)n + 1;
        size_t old_size = cache_size;

#ifndef __cplusplus
        /* C: realloc returns void*, cast to seq_t* */
        seq_t *new_cache = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache) abort();
        cache = new_cache;
        cache_size = new_size;

        /* Initialize newly allocated entries */
        for (size_t i = old_size; i < new_size; ++i) {
            if (i < 7)
                cache[i] = init_vals[i];
            else
                cache[i] = cache[i - 7] + 1;
        }
#else
        /* C++: use new[] which constructs objects */
        seq_t *new_cache = new seq_t[new_size];

        /* Copy existing values */
        for (size_t i = 0; i < old_size; ++i)
            new_cache[i] = cache[i];

        /* Initialize newly allocated entries */
        for (size_t i = old_size; i < new_size; ++i) {
            if (i < 7)
                new_cache[i] = init_vals[i];
            else
                new_cache[i] = new_cache[i - 7] + 1;
        }

        /* Free old cache */
        delete[] cache;
        cache = new_cache;
        cache_size = new_size;
#endif
    }
    return cache[n];
}
