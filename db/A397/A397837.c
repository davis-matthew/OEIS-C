/*
 * A397837  Number of ternary words of length n over {a,b,c} avoiding the
 *          consecutive word abcabc.
 * https://oeis.org/A397837
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) - a(n-3) + 3*a(n-4) - a(n-6), a(0)=1, a(1)=3,
 *            a(2)=9, a(3)=27, a(4)=81, a(5)=243, a(6)=728
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* static memoization cache */
    static seq_t *cache = NULL;   /* array of computed values */
    static size_t cap   = 0;      /* allocated capacity (in elements) */
    static idx_t  size  = 0;      /* number of entries already stored */

    /* Ensure the cache can hold n+1 entries */
    if ((size_t)(n + 1) > cap) {
        size_t new_cap = cap ? cap * 2 : 8;
        while (new_cap < (size_t)(n + 1))
            new_cap <<= 1;       /* double until sufficient */

        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();      /* allocation failed */
        cache = tmp;
        cap   = new_cap;
    }

    /* Compute all values up to n that are not already computed */
    for (idx_t i = size; i <= n; ++i) {
        if (i == 0)          cache[i] = 1;
        else if (i == 1)     cache[i] = 3;
        else if (i == 2)     cache[i] = 9;
        else if (i == 3)     cache[i] = 27;
        else if (i == 4)     cache[i] = 81;
        else if (i == 5)     cache[i] = 243;
        else if (i == 6)     cache[i] = 728;
        else
            cache[i] = 3 * cache[i - 1] - cache[i - 3]
                     + 3 * cache[i - 4] - cache[i - 6];
    }
    if (size <= n) size = n + 1;

    return cache[n];
}
