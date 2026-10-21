/*
 * A255607  Numbers n such that both 4*n+1 and 6*n+1 are primes.
 * https://oeis.org/A255607
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 630, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A255607: a(n) is the nth positive integer m such that 
   4*m+1 and 6*m+1 are primes.  The function computes values on
   demand and memoises them for later calls. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/isPrimeGeneric.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;          // undefined for n < 1

    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static size_t sz    = 0;
    static seq_t last_m = 0;     // largest candidate examined so far

    if (cache == NULL) {          // first call, allocate initial cache
        cap = 256;
        sz  = 0;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return 0;     // allocation failed
    }

    /* Extend cache until it contains at least n entries */
    while (sz < n) {
        last_m++;                                 // next candidate m

        seq_t v1 = (seq_t)4 * last_m + 1;         // 4*m+1
        seq_t v2 = (seq_t)6 * last_m + 1;         // 6*m+1

        if (isPrimeGeneric(v1) && isPrimeGeneric(v2)) {
            /* Store m in cache */
            if (sz == cap) {
                cap *= 2;
                seq_t *tmp = (seq_t *)realloc(cache, cap * sizeof(seq_t));
                if (!tmp) {
                    free(cache);
                    cache = NULL;
                    return 0;
                }
                cache = tmp;
            }
            cache[sz++] = last_m;
        }
    }

    /* Return the (n-1)-th element (0‑based indexing in array) */
    return cache[(size_t)(n - 1)];
}
