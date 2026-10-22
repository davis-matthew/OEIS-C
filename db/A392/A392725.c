/*
 * A392725  a(1) = 1; if the digit sum of |a(n)| is odd, then a(n+1) = a(n)-n,
 *          otherwise a(n+1) = a(n)+n.
 * https://oeis.org/A392725
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 42, i16 n <= 1050, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = 1; for n>=1,
   if digit_sum(|a(n)|) is odd then a(n+1)=a(n)-n,
   else a(n+1)=a(n)+n */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/digit_sum.h"

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static size_t len = 0;  /* number of stored terms */
    static size_t cap = 0;  /* capacity of cache */

    /* initialise cache with a(1) */
    if (len == 0) {
        cache = (seq_t*)malloc(sizeof(seq_t));
        if (!cache) abort();
        cache[0] = (seq_t)1;
        len = 1;
        cap = 1;
    }

    /* return cached value if already computed */
    if (n <= len)
        return cache[n - 1];

    /* ensure capacity */
    while (cap < n) {
        size_t new_cap = cap ? cap * 2 : 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cap = new_cap;
    }

    /* compute missing terms */
    for (idx_t i = len; i < n; ++i) {
        seq_t prev = cache[i - 1];
        seq_t abs_val = prev < 0 ? -prev : prev;
        seq_t ds = digit_sum(abs_val);
        if (ds & 1)       /* odd digit sum */
            cache[i] = prev - (seq_t)i;
        else              /* even digit sum */
            cache[i] = prev + (seq_t)i;
    }

    len = n;
    return cache[n - 1];
}
