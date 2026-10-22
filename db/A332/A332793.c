/*
 * A332793  a(1) = 1; a(n) = n * Sum_{d|n, d < n} (-1)^(n/d) * a(d) / d.
 * https://oeis.org/A332793
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   a(n) = n * Sum_{d|n, d < n} (-1)^(n/d) * a(d)/d with a(1) = 1
 * Range:     i8 n <= 15, i16 n <= 255, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    static seq_t *cache   = NULL;   /* values a(i) */
    static bool *cached   = NULL;   /* whether a(i) is computed */
    static idx_t  allocated = 0;    /* size of the arrays */

    /* Grow the arrays until they can hold index n. */
    if (n >= allocated) {
        idx_t new_size = (allocated == 0) ? 64 : allocated;
        while (new_size <= n)
            new_size *= 2;

        /* In both C and C++ realloc returns void*, so cast explicitly. */
        seq_t *new_cache   = (seq_t*)realloc(cache,   new_size * sizeof(seq_t));
        bool   *new_cached = (bool*)realloc(cached,   new_size * sizeof(bool));

        cache   = new_cache;
        cached  = new_cached;

        /* Zero the newly added cached flags.  */
        if (new_cached != NULL)
            memset(new_cached + allocated, 0, (new_size - allocated) * sizeof(bool));

        allocated = new_size;
    }

    /* Base case.  */
    if (!cached[1]) {
        cache[1] = (seq_t)1;
        cached[1] = true;
    }

    /* Compute values up to n, if necessary. */
    for (idx_t i = 2; i <= n; ++i) {
        if (cached[i])  /* already computed */
            continue;

        seq_t sum = 0;
        for (idx_t d = 1; d <= i / d; ++d) {        /* iterate over divisors d <= sqrt(i) */
            if (i % d != 0)
                continue;

            idx_t d2 = i / d;                       /* complementary divisor */

            /* divisor d, d < i by construction because i>1 and d <= i/d */
            if (d < i) {
                seq_t expo = (seq_t)(i / d);           /* quotient n/d */
                int   sign = ((expo & 1) == 0) ? 1 : -1;
                seq_t term = (seq_t)sign * (cache[d] / (seq_t)d);
                sum += term;
            }

            /* divisor d2, distinct from d and < i */
            if (d2 != d && d2 < i) {
                seq_t expo = (seq_t)(i / d2);          /* which equals d */
                int   sign = ((expo & 1) == 0) ? 1 : -1;
                seq_t term = (seq_t)sign * (cache[d2] / (seq_t)d2);
                sum += term;
            }
        }

        cache[i]   = (seq_t)i * sum;
        cached[i]  = true;
    }

    return cache[n];
}
