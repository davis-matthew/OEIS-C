/*
 * A370768  Number of signed permutations of length n+2 with adjacent elements
 *          differing by more than 1 whose first element is 1 and whose last
 *          element has absolute value n+2.
 * https://oeis.org/A370768
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*n*a(n-1) + 4*a(n-2) + 4*(2-n)*a(n-3) + a(n-4) -
 *            2*(2-n)*a(n-5) Simplified to avoid negative multipliers: a(n) =
 *            2*n*a(n-1) + 4*a(n-2) - 4*(n-2)*a(n-3) + a(n-4) + 2*(n-2)*a(n-5)
 *            with a(0)=1, a(1)=1, a(2)=5, a(3)=29, a(4)=249
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


seq_t a(idx_t n)
{
    /* base values for n = 0..4 */
    static const seq_t base[5] = { (seq_t)1, (seq_t)1, (seq_t)5, (seq_t)29, (seq_t)249 };

    /* cache for already computed terms */
    static seq_t *cache   = NULL;
    static idx_t  size    = 0;    /* number of elements currently cached */

    /* if n is a small index, use the base array */
    if (n < 5) {
        return base[(size_t)n];
    }

    /* initialise cache with the first five terms if necessary */
    if (cache == NULL) {
        size = 5;
        cache = (seq_t *)malloc(size * sizeof(seq_t));
        if (cache == NULL) { /* fallback: compute directly (should not happen) */
            return base[(size_t)n];
        }
        for (idx_t i = 0; i < 5; ++i) {
            cache[i] = base[i];
        }
    }

    /* grow cache to accommodate the requested index n */
    if (n >= size) {
        /* grow to size n+1 */
        seq_t *new_cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (new_cache == NULL) {
            /* allocation failed – return 0 as a sentinel (should not happen) */
            return (seq_t)0;
        }
        cache = new_cache;
        /* compute new terms from the current cached size up to n */
        for (idx_t i = size; i <= n; ++i) {
            /* compute recurrence components */
            seq_t term1 = 2 * (seq_t)i * cache[i - 1];
            seq_t term2 = 4 * cache[i - 2];
            seq_t term4 = cache[i - 4];
            seq_t term5 = 2 * (seq_t)(i - 2) * cache[i - 5];
            seq_t negative = 4 * (seq_t)(i - 2) * cache[i - 3];

            /* sum positives and subtract the negative contribution */
            seq_t sum = term1 + term2 + term4 + term5;
            cache[i]  = sum - negative;
        }
        size = n + 1;
    }

    return cache[n];
}
