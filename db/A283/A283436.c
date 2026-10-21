/*
 * A283436  Hankel transform of A191529.
 * https://oeis.org/A283436
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 20, i16 n <= 40, i32 n <= 40, i64 n <= 40, i128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A283436: Hankel transform of A191529
   Recurrence: a(n) = a(n-2) + a(n-3) - a(n-5) + a(n-6) - a(n-8) - a(n-9) + a(n-11)
   for n > 10, with initial values a(0)=1, a(1)=1, a(2)=1, a(3)=1,
   a(4)=-2, a(5)=-3, a(6)=-10, a(7)=-10, a(8)=-17, a(9)=-20,
   a(10)=-27, a(11)=-31. */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static cache: values already computed */
    static seq_t *cache   = NULL;   /* pointer to array of seq_t elements */
    static size_t  cap    = 0;      /* allocated array length (in elements) */
    static idx_t   max_cached = 0;  /* exclusive upper bound of computed values */

    /* first time: initialise cache with the 12 known values (indices 0..11) */
    if (cache == NULL) {
        cap = 12;                     /* allocate space for 12 indices */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (cache == NULL) return 0;   /* allocation failure */

        cache[0]  =  1;  cache[1]  =  1;
        cache[2]  =  1;  cache[3]  =  1;
        cache[4]  = -2;  cache[5]  = -3;
        cache[6]  = -10; cache[7]  = -10;
        cache[8]  = -17; cache[9]  = -20;
        cache[10] = -27; cache[11] = -31;

        max_cached = 12;                /* values 0..11 have been computed */
    }

    /* If requested value is already in the cache, return it. */
    if (n < max_cached)
        return cache[n];

    /* Ensure the array is large enough for index n. */
    size_t required = (size_t)n + 1;     /* indices 0..n inclusive */
    if (required > cap) {
        size_t newcap = cap;
        while (newcap < required)
            newcap <<= 1;                /* double until large enough */
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (tmp == NULL) return 0;      /* allocation failure */
        cache = tmp;
        cap   = newcap;
    }

    /* Compute all missing terms up to n, using the recurrence. */
    for (idx_t i = max_cached; i <= n; ++i) {
        seq_t val = cache[i-2] + cache[i-3]
                  - cache[i-5] + cache[i-6]
                  - cache[i-8] - cache[i-9]
                  + cache[i-11];
        cache[i] = val;
    }
    max_cached = n + 1;                  /* newly computed indices are now cached */

    return cache[n];
}
