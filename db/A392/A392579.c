/*
 * A392579  Expansion of 1 / ((1-x)^4 - x)^2.
 * https://oeis.org/A392579
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 14, u64 n <= 31, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A392579 */
#include <stdlib.h>

/* a(n) = Sum_{k=0..n} (k+1) * binomial(n+3k+7, n-k)
   Recurrence:
      a(n) = 10*a(n-1) - 37*a(n-2) + 68*a(n-3) - 78*a(n-4)
              + 58*a(n-5) - 28*a(n-6) + 8*a(n-7) - a(n-8)
   with a(0)=1, a(1)=10, a(2)=63, a(3)=328, a(4)=1551,
   a(5)=6936, a(6)=29915, a(7)=125784 */
static const seq_t base_vals[8] = {
    (seq_t)1,        (seq_t)10,
    (seq_t)63,       (seq_t)328,
    (seq_t)1551,     (seq_t)6936,
    (seq_t)29915,    (seq_t)125784
};

static seq_t *cache = NULL;    /* array of computed a(n) */
static size_t cache_len = 0;   /* number of terms stored in cache */

seq_t a(idx_t n)
{
    /* handle the first eight values directly */
    if (n < 8)
        return base_vals[(size_t)n];

    /* initialise cache if it does not exist */
    if (cache == NULL) {
        cache_len = 8;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (!cache) return (seq_t)0;                /* allocation failed */
        for (size_t i = 0; i < 8; ++i)
            cache[i] = base_vals[i];
    }

    /* remember the current cache length before a possible re‑allocation */
    size_t old_len = cache_len;

    /* enlarge cache so that it can hold a(n) */
    if ((size_t)n + 1 > cache_len) {
        size_t new_len = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) return (seq_t)0;                  /* re‑allocation failed */
        cache   = tmp;
        cache_len = new_len;
    }

    /* compute missing terms up to n, starting from the old length */
    for (idx_t i = old_len; i <= n; ++i) {
        /* pos terms: 10*a(i-1) + 68*a(i-3) + 58*a(i-5) + 8*a(i-7)   */
        seq_t pos = (seq_t)10 * cache[(size_t)(i - 1)];
        pos += (seq_t)68 * cache[(size_t)(i - 3)];
        pos += (seq_t)58 * cache[(size_t)(i - 5)];
        pos += (seq_t)8  * cache[(size_t)(i - 7)];

        /* neg terms: 37*a(i-2) + 78*a(i-4) + 28*a(i-6) + 1*a(i-8)   */
        seq_t neg = (seq_t)37 * cache[(size_t)(i - 2)];
        neg += (seq_t)78 * cache[(size_t)(i - 4)];
        neg += (seq_t)28 * cache[(size_t)(i - 6)];
        neg +=       cache[(size_t)(i - 8)];          /* coefficient -1 */

        cache[(size_t)i] = pos - neg;
    }

    return cache[(size_t)n];
}
