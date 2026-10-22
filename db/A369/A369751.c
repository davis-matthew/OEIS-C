/*
 * A369751  Expansion of e.g.f. exp(1 - (1+x)^3).
 * https://oeis.org/A369751
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n)+3*a(n-1)+6*(n-1)*a(n-2)+3*(n-1)*(n-2)*a(n-3)=0, a(0)=1,
 *            a(1)=-3, a(2)=3
 * Range:     i8 n <= 4, i16 n <= 7, i32 n <= 12, i64 n <= 21, i128 n <= 22,
 *            big any n
 * Helpers:   seq_alloc_new_array, seq_free
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* size_t */
#include <string.h>   /* memcpy */
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_free.h"

seq_t a(idx_t n)
{
    /* static memoisation cache */
    static seq_t *cache = NULL;
    static size_t cap   = 0;   /* allocated size in elements */
    static size_t len   = 0;   /* number of terms currently stored */

    /* initialise base cases once */
    if (len == 0) {
        cap = 3;
        cache = seq_alloc_new_array(cap);
        if (!cache) return 0;      /* allocation failure – return zero */
        cache[0] = 1;
        cache[1] = -3;
        cache[2] = 3;
        len = 3;
    }

    /* if requested index already computed, return it */
    if (n < (idx_t)len) {
        return cache[n];
    }

    /* extend cache if necessary */
    size_t need = (size_t)n + 1;
    if (need > cap) {
        seq_t *tmp = seq_alloc_new_array(need);
        if (!tmp) return 0;       /* allocation failure – return zero */
        memcpy(tmp, cache, len * sizeof(seq_t));
        seq_free(cache);
        cache = tmp;
        cap   = need;
    }

    /* compute missing terms up to n */
    for (idx_t i = (idx_t)len; i <= n; ++i) {
        /* intermediate values for (i-1) and (i-2) as seq_t */
        seq_t di1 = (seq_t)(i - 1);
        seq_t di2 = (seq_t)(i - 2);

        /* recurrence rhs */
        seq_t t1 = 3 * cache[i - 1];
        seq_t t2 = 6 * di1 * cache[i - 2];
        seq_t t3 = 3 * di1 * di2 * cache[i - 3];

        cache[i] = -(t1 + t2 + t3);
    }

    len = need;
    return cache[n];
}
