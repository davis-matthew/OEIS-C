/*
 * A249190  Number of length n+6 0..1 arrays with no seven consecutive terms
 *          having four times the sum of any three elements equal to three
 *          times the sum of the remaining four.
 * https://oeis.org/A249190
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1)+a(n-2)+a(n-3)+a(n-4)+a(n-5)+a(n-6) with a(1)=126,
 *            a(2)=250, a(3)=496, a(4)=984, a(5)=1952, a(6)=3872. This
 *            recurrence holds for all n ≥ 7
 * Range:     u8 n <= 2, u16 n <= 10, u32 n <= 26, u64 n <= 58, u128 n <= 123,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache: 1‑based index → 0‑based array */
    static seq_t *cache = NULL;
    static idx_t cacheSize = 0;

    if (n < 1)   /* out‑of‑range; behaviour unspecified */
        return (seq_t)0;

    /* Grow cache if necessary */
    if (cache == NULL) {
        cacheSize = 0;
    }

    if (n > cacheSize) {
        /* Allocate or enlarge the cache */
        size_t newSize = (size_t)n;
        seq_t *tmp = (seq_t *)realloc(cache, newSize * sizeof(seq_t));
        if (tmp == NULL) { /* allocation failure – return 0 */
            free(cache);
            cache = NULL;
            cacheSize = 0;
            return (seq_t)0;
        }
        cache = tmp;

        /* Initialize required terms */
        for (idx_t i = cacheSize + 1; i <= n; ++i) {
            seq_t val;
            if (i == 1)      val = (seq_t)126;
            else if (i == 2) val = (seq_t)250;
            else if (i == 3) val = (seq_t)496;
            else if (i == 4) val = (seq_t)984;
            else if (i == 5) val = (seq_t)1952;
            else if (i == 6) val = (seq_t)3872;
            else {
                /* Sum of the previous six terms */
                val = cache[(i-2)   ] +   /* a(i-1) */
                      cache[(i-3)   ] +   /* a(i-2) */
                      cache[(i-4)   ] +   /* a(i-3) */
                      cache[(i-5)   ] +   /* a(i-4) */
                      cache[(i-6)   ] +   /* a(i-5) */
                      cache[(i-7)   ];   /* a(i-6) */
            }
            cache[(size_t)(i-1)] = val;
        }
        cacheSize = n;
    }

    return cache[(size_t)(n-1)];
}
