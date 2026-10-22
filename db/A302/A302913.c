/*
 * A302913  Determinant of n X n matrix whose main diagonal consists of the
 *          first n 9-gonal numbers and all other elements are 1's.
 * https://oeis.org/A302913
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n+1) = a(n) * n*(7*n+9)/2 with a(1) = 1
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of OEIS sequence A302913.
   The function is memoised so that repeated calls are fast. */

#include <stdlib.h>

static seq_t *cache = NULL;      /* 1‑based cache: cache[i] = a(i) */
static idx_t cache_max = 0;      /* highest index currently stored */

seq_t a(idx_t n)
{
    if (n < 1) return 0;        /* undefined for n < 1 */

    /* initialise cache for a(1) if necessary */
    if (cache_max < 1) {
        cache = (seq_t *)malloc(sizeof(seq_t) * 2);   /* indices 0 and 1 */
        if (!cache) return 0;                         /* allocation failure */
        cache[0] = (seq_t)0;                          /* unused */
        cache[1] = (seq_t)1;                          /* a(1) = 1 */
        cache_max = 1;
    }

    /* extend cache up to n */
    if (n > cache_max) {
        size_t new_cap = (size_t)n + 1;   /* need indices 0..n */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;               /* allocation failure */
        cache = tmp;

        for (idx_t i = cache_max + 1; i <= n; ++i) {
            idx_t k = i - 1;                       /* recurrence uses n = k */
            seq_t kk   = (seq_t)k;                 /* type seq_t for exactness */
            seq_t step = kk * ( (seq_t)7 * kk + (seq_t)9 ); /* n*(7*n+9) */
            step /= (seq_t)2;                      /* divide by 2 (always even) */
            cache[i] = cache[i - 1] * step;        /* a(i) = a(i-1) * step */
        }
        cache_max = n;
    }

    return cache[n];
}
