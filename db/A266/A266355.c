/*
 * A266355  Expansion of b(2)*b(4)*b(6)/(1-x-x^2-x^4+x^6+x^8), where b(k) =
 *          (1-x^k)/(1-x).
 * https://oeis.org/A266355
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 17, u32 n <= 38, u64 n <= 80, u128 n <= 164,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A266355
   a(0) = 1, a(1) = 4, a(2) = 10, a(3) = 21, a(4) = 40, a(5) = 73,
   a(6) = 129, a(7) = 224, a(8) = 385, a(9) = 658
   Recurrence (for n ≥ 10):
   a(n) = a(n-1) + a(n-2) + a(n-4) - a(n-6) - a(n-8)            */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Initial 10 values */
    static const seq_t init[10] = {
        (seq_t) 1,
        (seq_t) 4,
        (seq_t)10,
        (seq_t)21,
        (seq_t)40,
        (seq_t)73,
        (seq_t)129,
        (seq_t)224,
        (seq_t)385,
        (seq_t)658
    };

    /* Cache for memoisation */
    static seq_t *cache = NULL;      /* computed up to max_n */
    static size_t cache_size = 0;    /* allocated size */
    static idx_t max_n_computed = (idx_t)-1; /* last index with value */

    /* Fast return for n < 10 */
    if (n < 10)
        return init[n];

    /* Initialise cache on first call */
    if (cache == NULL) {
        cache_size = (size_t)(9 + 1);           /* store indices 0..9 */
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        for (idx_t i = 0; i < 10; ++i)
            cache[i] = init[i];
        max_n_computed = 9;
    }

    /* Grow cache if needed */
    if ((idx_t)cache_size <= n) {
        size_t new_size = cache_size * 2;
        if (new_size <= (size_t)n)
            new_size = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (tmp == NULL) /* allocation failed – fall back to minimal array */
            return 0;
        cache = tmp;
        cache_size = new_size;
    }

    /* Compute values up to n */
    for (idx_t i = max_n_computed + 1; i <= n; ++i) {
        seq_t t = cache[i-1] + cache[i-2] + cache[i-4];
        t -= cache[i-6];
        t -= cache[i-8];
        cache[i] = t;
    }
    max_n_computed = n;

    return cache[n];
}
