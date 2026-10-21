/*
 * A248505  Alternating the subtraction and multiplication of two previous
 *          terms, starting with 3, 2.
 * https://oeis.org/A248505
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(1) = 3, a(2) = 2; For n odd: a(n) = a(n-1) - a(n-2); For n
 *            even: a(n) = a(n-1) * a(n-2);
 * Range:     i8 n <= 11, i16 n <= 13, i32 n <= 15, i64 n <= 17, i128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute A248505 for index n (1‑based).  Results are cached
   in a static array that grows as needed.  Works for any
   seq_t and idx_t supplied by the build. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of already computed terms, 1‑indexed but stored zero‑based  */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;      /* number of slots currently allocated */

    if (n == 0)              /* offset < 1 is irrelevant per specification */
        return 0;

    /* Ensure at least two entries exist (a(1) and a(2)). */
    if (cache_len < 2) {
        idx_t need = 2;
        seq_t *tmp = (seq_t *)realloc(cache, need * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        cache = tmp;
        if (cache_len == 0) cache[0] = (seq_t)3;
        if (cache_len <= 1) cache[1] = (seq_t)2;
        cache_len = 2;
    }

    /* Grow the cache if we need more terms.  */
    if (cache_len < n) {
        seq_t *tmp = (seq_t *)realloc(cache, n * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        cache = tmp;

        for (idx_t i = cache_len; i < n; ++i) {
            idx_t idx = i + 1;                    /* actual sequence index (1‑based) */
            if (idx & 1U)                        /* n odd */
                cache[i] = cache[i - 1] - cache[i - 2];
            else                                 /* n even */
                cache[i] = cache[i - 1] * cache[i - 2];
        }
        cache_len = n;
    }

    return cache[n - 1];
}
