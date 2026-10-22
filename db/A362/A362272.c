/*
 * A362272  a(1) = 1, then multiply, subtract, and add 2, 3, 4; 5, 6, 7; ...
 *          in that order.
 * https://oeis.org/A362272
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(1)=1; for n>1 a(n) = a(n-1)*n if n mod 3 = 2, a(n-1)-n if n
 *            mod 3 = 0, a(n-1)+n if n mod 3 = 1
 * Range:     i8 n <= 7, i16 n <= 16, i32 n <= 25, i64 n <= 33, i128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* offset is 1; not used */

    /* Persistent cache */
    static seq_t *cache = NULL;   /* array of a(1) .. a(used)      */
    static idx_t cap   = 0;       /* current allocated capacity   */
    static idx_t used  = 0;       /* number of terms stored       */

    /* initialise cache on first call */
    if (cache == NULL) {
        cap   = 64;
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        cache[0] = (seq_t)1;      /* a(1) = 1                         */
        used  = 1;
    }

    /* grow cache if necessary */
    if (n > cap) {
        idx_t new_cap = cap;
        while (new_cap < n) new_cap <<= 1;       /* double until enough */
        cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        cap   = new_cap;
    }

    /* compute new terms up to n */
    if (n > used) {
        for (idx_t i = used + 1; i <= n; ++i) {
            seq_t prev = cache[i - 2];          /* a(i-1) */
            seq_t inc  = (seq_t)i;              /* i as seq_t */
            idx_t mod = i % 3;
            if (mod == 2) {
                cache[i - 1] = prev * inc;      /* a(i) = a(i-1) * i */
            } else if (mod == 0) {
                cache[i - 1] = prev - inc;      /* a(i) = a(i-1) - i */
            } else { /* mod == 1 */
                cache[i - 1] = prev + inc;      /* a(i) = a(i-1) + i */
            }
        }
        used = n;
    }

    return cache[n - 1];   /* a(n) stored at index n-1 (zero‑based) */
}
