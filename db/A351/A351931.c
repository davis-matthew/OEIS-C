/*
 * A351931  Expansion of e.g.f. exp(x - x^5/120).
 * https://oeis.org/A351931
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0)=a(1)=a(2)=a(3)=a(4)=1,
 * Range:     i8 n <= 6, i16 n <= 12, i32 n <= 20, i64 n <= 28, i128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
// a(n) = a(n-1) - C(n-1,4) * a(n-5) for n > 4
// where C(n-1,4) = (n-1)(n-2)(n-3)(n-4)/24.
*/
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Small indices are trivial. */
    if (n < 5) return (seq_t)1;

    /* Static cache for memoisation. */
    static seq_t *cache = NULL;
    static idx_t last_computed = 4;      /* last index computed in cache */

    /* Lazily initialise the cache with the base values. */
    if (!cache) {
        cache = (seq_t *)malloc(5 * sizeof(seq_t));
        for (idx_t i = 0; i <= 4; ++i)
            cache[i] = (seq_t)1;
        last_computed = 4;
    }

    /* Grow the cache and compute new terms if necessary. */
    if (n > last_computed) {
        cache = (seq_t *)realloc(cache, (size_t)(n + 1) * sizeof(seq_t));

        for (idx_t i = last_computed + 1; i <= n; ++i) {
            /* Compute binomial coefficient C(i-1,4). */
            seq_t binom = (seq_t)(i - 1);
            binom *= (seq_t)(i - 2);
            binom *= (seq_t)(i - 3);
            binom *= (seq_t)(i - 4);
            binom /= (seq_t)24;

            /* Apply the recurrence. */
            cache[i] = cache[i - 1] - binom * cache[i - 5];
        }
        last_computed = n;
    }

    return cache[n];
}
