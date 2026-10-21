/*
 * A259775  Stepped path in P(k,n) array of k-th partial sums of squares
 *          (A000290).
 * https://oeis.org/A259775
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 19, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  
   Recurrence (conjecture):
     a(1)=1, a(2)=5, a(3)=6
     For n≥4:
        a(n) = ( (8*n^2+39*n-35) * a(n-1)
                 + 2*(26*n^2+48*n+25) * a(n-2)
                 - 4*(8*n+5)*(n-1) * a(n-3) ) 
              / ( (n+5)*(13*n-11) )
   All arithmetic is performed in seq_t, the build‑specific integer type.
*/
#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation; 0‑th element is unused */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* allocated capacity (in elements) */
    static size_t cache_size = 0;  /* index of first uninitialised element */

    /* Ensure cache can hold up to n (1‑based) */
    if (cache_cap <= (size_t)n) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        while (new_cap <= (size_t)n) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Return already computed value */
    if (n < cache_size) return cache[(size_t)n];

    /* Compute missing values up to n */
    for (size_t i = cache_size; i <= (size_t)n; ++i) {
        if (i == 1) {
            cache[1] = (seq_t)1;
        } else if (i == 2) {
            cache[2] = (seq_t)5;
        } else if (i == 3) {
            cache[3] = (seq_t)6;
        } else {
            seq_t ns = (seq_t)i;
            seq_t coeff1 = 8 * ns * ns + 39 * ns - 35;
            seq_t coeff2 = 2 * (26 * ns * ns + 48 * ns + 25);
            seq_t coeff3 = 4 * (8 * ns + 5) * (ns - 1);

            seq_t term1 = coeff1 * cache[i - 1];
            seq_t term2 = coeff2 * cache[i - 2];
            seq_t term3 = coeff3 * cache[i - 3];

            seq_t numerator   = term1 + term2 - term3;
            seq_t denominator = (ns + 5) * (13 * ns - 11);

            cache[i] = numerator / denominator;
        }
    }

    cache_size = (size_t)n + 1;
    return cache[(size_t)n];
}
