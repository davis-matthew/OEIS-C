/*
 * A375364  Expansion of 1/( (1 + x) * (1 - x^2*(1 + x)^3) ).
 * https://oeis.org/A375364
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = -a(n-1)+a(n-2)+4*a(n-3)+6*a(n-4)+4*a(n-5)+a(n-6); a(0)=1,
 *            a(1)=-1, a(2)=2, a(3)=1, a(4)=3, a(5)=4
 * Range:     i8 n <= 10, i16 n <= 18, i32 n <= 36, i64 n <= 36, i128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    static const seq_t base[6] = {1, -1, 2, 1, 3, 4};

    if (n < 6)
        return base[n];

    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static idx_t max_computed = (idx_t)-1; /* –1 means not yet initialized */

    /* Allocate or grow the cache if necessary */
    if (!cache) {
        cap = (size_t)(n + 1);
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        if (!cache) abort();          /* allocation failure */
        for (idx_t i = 0; i < 6; ++i)
            cache[i] = base[i];
        max_computed = 5;
    } else if ((size_t)(n + 1) > cap) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cap = new_cap;
    }

    if (n <= max_computed)
        return cache[n];

    /* Compute missing terms using the recurrence */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        cache[i] = -cache[i-1] + cache[i-2]
                   + 4 * cache[i-3] + 6 * cache[i-4]
                   + 4 * cache[i-5] + cache[i-6];
    }
    max_computed = n;
    return cache[n];
}
