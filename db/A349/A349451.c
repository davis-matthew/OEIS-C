/*
 * A349451  Dirichlet inverse of Fibonacci numbers, when started from
 *          A000045(1): 1, 1, 2, 3, 5, 8, 13, 21, ...
 * https://oeis.org/A349451
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 12, i16 n <= 23, i32 n <= 46, i64 n <= 92, i128 n <= 184,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1; for n>1, a(n) = -Σ_{d|n, d<n} Fibonacci(n/d) * a(d) */
#include <stddef.h>
#include <stdlib.h>
#include "../utils/fib.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;               /* undefined for n=0 */

    /* static memoization array */
    static seq_t *cache = NULL;
    static size_t cap   = 0;     /* capacity (elements) */
    static idx_t  max   = 0;     /* greatest index cached so far */

    /* enlarge allocation if needed */
    if (n + 1 > cap) {
        size_t newcap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cap = newcap;
    }

    /* if already computed, return directly */
    if (n <= max) return cache[n];

    /* compute missing values in increasing order */
    for (idx_t i = max + 1; i <= n; ++i) {
        if (i == 1) {
            cache[1] = 1;
            continue;
        }

        seq_t sum = 0;
        for (idx_t d = 1; d * d <= i; ++d) {
            if (i % d) continue;

            idx_t d2 = i / d;

            if (d < i) {
                seq_t f = fib(i / d);
                sum += f * cache[d];
            }
            if (d2 != d && d2 < i) {
                seq_t f = fib(i / d2);
                sum += f * cache[d2];
            }
        }
        cache[i] = -sum;
    }

    max = n;
    return cache[n];
}
