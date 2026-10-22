/*
 * A346118  a(1) = 1; a(n+1) = 1 + Sum_{d|n} mu(n/d) * a(d).
 * https://oeis.org/A346118
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n+1) = 1 + Sum_{d|n} mu(n/d) * a(d), a(1) = 1
 * Range:     i8 n <= 64, i16 n <= 70, i32 n <= 70, i64 n <= 70, i128 n <= 70,
 *            big any n
 * Helpers:   mobius
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation memoises computed terms and uses a simple
   divisor loop to evaluate the sum.  It works for all provided
   utils, including the arbitrary‑precision build. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mobius.h"          /* provides seq_t mobius(idx_t m) */

static seq_t *cache = NULL;          /* cache[1..] contains a(i) */
static size_t cache_cap = 0;         /* number of allocated slots (>= max i+1) */
static idx_t computed_max = 0;       /* highest n that has been computed */

/* Ensure that cache has capacity for indices up to n (inclusive). */
static void ensure_capacity(idx_t n)
{
    if (cache_cap >= (size_t)(n + 1))
        return;                      /* already enough */

    /* Reallocate to the required size.  Cast the result of realloc. */
    seq_t *tmp = (seq_t *)realloc(cache, (size_t)(n + 1) * sizeof(seq_t));
    if (!tmp)                        /* allocation failed */
        abort();

    cache = tmp;
    /* Initialise newly allocated entries to 0.  For integral types this
       assigns 0; for a big‑integer class this invokes the constructor
       from 0, which is provided. */
    for (size_t i = cache_cap; i <= (size_t)n; ++i)
        cache[i] = (seq_t)0;

    cache_cap = (size_t)(n + 1);
}

/* External linkage required by the build system. */
seq_t a(idx_t n)
{
    if (n == 0)                       /* undefined, return 0 */
        return (seq_t)0;

    /* Lazy initialisation of the cache. */
    if (cache == NULL) {
        ensure_capacity(1);           /* need at least index 1 */
        cache[1] = (seq_t)1;          /* a(1) = 1 */
        computed_max = 1;
    }

    /* Compute all terms up to n if they are not already cached. */
    if (n > computed_max) {
        ensure_capacity(n);

        for (idx_t i = computed_max + 1; i <= n; ++i) {
            idx_t m = i - 1;          /* n in the recurrence */
            seq_t sum = (seq_t)0;

            /* Iterate divisors of m. */
            for (idx_t d = 1; (unsigned long long)d * d <= m; ++d) {
                if (m % d == 0) {
                    idx_t d1 = d;
                    idx_t d2 = m / d;

                    /* Term for divisor d1. */
                    seq_t mu1 = mobius(m / d1);
                    sum += mu1 * cache[d1];

                    if (d1 != d2) {   /* d2 distinct from d1 */
                        seq_t mu2 = mobius(m / d2);
                        sum += mu2 * cache[d2];
                    }
                }
            }
            cache[i] = (seq_t)1 + sum;
        }
        computed_max = n;
    }

    return cache[n];
}
