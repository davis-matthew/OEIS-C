/*
 * A249043  a(1) = 42; a(n+1) = a(n) + sum of decimal digits of a(n).
 * https://oeis.org/A249043
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(1) = 42; a(n+1) = a(n) + digit_sum(a(n))
 * Range:     u8 n <= 25, u16 n <= 3273, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/digit_sum.h"

/* External linkage required by the test harness */
seq_t a(idx_t n)
{
    /* constants and static state for memoisation */
    static seq_t *cache = NULL;   /* 1‑indexed array of computed values */
    static size_t capacity = 0;   /* allocated size of cache */
    static idx_t last = 0;        /* largest n such that cache[n] is valid */

    /* Basic guard against zero index (unspecified behaviour) */
    if (n == 0)
        return (seq_t)0;

    /* Resize cache if necessary */
    if ((size_t)(n + 1) > capacity) {
        size_t new_cap = (size_t)(n + 1);
        /* Allocate new array larger than needed */
        seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!new_cache) return (seq_t)0;   /* OOM → undefined behaviour */

        /* Copy existing values */
        for (idx_t i = 1; i <= last; ++i)
            new_cache[i] = cache[i];

        /* Free old array and update pointers */
        free(cache);
        cache = new_cache;
        capacity = new_cap;
    }

    /* Initialise first term on first call */
    if (last == 0) {
        cache[1] = (seq_t)42;
        last = 1;
    }

    /* Compute any missing terms up to n */
    for (idx_t i = last + 1; i <= n; ++i) {
        cache[i] = cache[i - 1] + digit_sum(cache[i - 1]);
    }
    last = n;

    return cache[n];
}
