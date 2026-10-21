/*
 * A293123  E.g.f.: exp(-x^3/(1+x)).
 * https://oeis.org/A293123
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 5, i16 n <= 7, i32 n <= 11, i64 n <= 19, i128 n <= 32,
 *            big any n
 * Helpers:   free_cache, seq_alloc_new_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Formula (recurrence):
   a(0)=1, a(1)=0, a(2)=0, a(3)=-6
   a(n) + 2*(n-1)*a(n-1) + (n-1)*(n-2)*a(n-2) + 3*(n-1)*(n-2)*a(n-3)
        + 2*(n-1)*(n-2)*(n-3)*a(n-4) = 0      for n>=4
*/

#include <stdlib.h>
#include <string.h>
#include "../utils/seq_alloc_new_array.h"
#include "../utils/free_cache.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t capacity = 0;       /* allocated size (number of slots)   */
    static size_t max_computed = 0;   /* highest index whose value is stored */

    /* Initialisation – allocate and fill first four entries. */
    if (!cache)
    {
        capacity = 4;                         /* indices 0..3 -> 4 slots          */
        cache = seq_alloc_new_array(capacity); /* allocate `capacity` seq_t items  */
        if (!cache) return 0;                 /* allocation failure                */

        cache[0] = (seq_t)1;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)0;
        cache[3] = (seq_t)-6;
        max_computed = 3;
    }

    /* Ensure enough space for index n. */
    if ((size_t)n >= capacity)
    {
        size_t new_cap = capacity;
        while (new_cap <= (size_t)n) new_cap <<= 1;   /* double until big enough */

        seq_t *new_cache = seq_alloc_new_array(new_cap);
        if (!new_cache) return 0;

        memcpy(new_cache, cache, capacity * sizeof(seq_t));
        free_cache(cache);
        cache = new_cache;
        capacity = new_cap;
    }

    /* Compute values up to n, if not already done. */
    for (size_t k = max_computed + 1; k <= (size_t)n; ++k)
    {
        seq_t val;
        if (k <= 3)            /* safety, should not happen */
            val = cache[k];
        else
        {
            seq_t n1 = (seq_t)(k - 1);
            seq_t n2 = (seq_t)(k - 2);
            seq_t n3 = (seq_t)(k - 3);

            /* a(k) = -( 2*n1*a[k-1] + n1*n2*a[k-2] + 3*n1*n2*a[k-3] + 2*n1*n2*n3*a[k-4] ) */
            val = -(
                    (seq_t)2 * n1 * cache[k - 1]
                  + n1 * n2 * cache[k - 2]
                  + (seq_t)3 * n1 * n2 * cache[k - 3]
                  + (seq_t)2 * n1 * n2 * n3 * cache[k - 4]
                 );
        }
        cache[k] = val;
        max_computed = k;
    }

    return cache[n];
}
