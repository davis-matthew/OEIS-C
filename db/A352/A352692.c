/*
 * A352692  a(n) + a(n+1) = 2^n for n >= 0 with a(0) = 4.
 * https://oeis.org/A352692
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(0) = 4; a(n)+a(n+1) = 2^n for n >= 0 (OEIS A352692)
 * Range:     i8 n <= 8, i16 n <= 16, i32 n <= 32, i64 n <= 36, i128 n <= 36,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/pow2.h"

static seq_t *cache = NULL;      /* array of computed a(n) values        */
static idx_t  cache_cap = 0;     /* number of elements stored in cache   */

seq_t a(idx_t n)
{
    /* Initialise cache with a(0) on first call        */
    if (cache == NULL) {
        cache      = (seq_t *)malloc(sizeof(seq_t));   /* allocate space for a(0) */
        cache_cap  = 1;
        cache[0]   = (seq_t)4;                         /* a(0) = 4                */
    }

    /* Extend cache if requested index not yet present   */
    if (n >= cache_cap) {
        idx_t new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) {                       /* allocation failure        */
            return 0;                            /* unlikely for n ~ 10⁴      */
        }
        cache = tmp;

        /* Compute missing values using the recurrence
           a(i+1) = 2^i - a(i)   for i = cache_cap-1 .. n-1 */
        for (idx_t i = cache_cap - 1; i < n; ++i) {
            seq_t two_pow = pow2(i);              /* 2^i as seq_t              */
            cache[i + 1] = two_pow - cache[i];
        }
        cache_cap = new_cap;
    }

    return cache[n];
}
