/*
 * A304908  Expansion of x * (d/dx) 1/(1 - Sum_{k>=0} x^(2^k)).
 * https://oeis.org/A304908
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*A023359(n) where A023359(n) = number of compositions of
 *            n into powers of 2, with A023359(0) = 1 and for n > 0:
 *            A023359(n) = Σ A023359(n‑2^k) (sum over powers of 2 ≤ n). Thus
 *            a(n) = n * A023359(n). a(0) = 0
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 33, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The algorithm uses a memoised DP array so the value for each n is
   computed only once.  Complexity: O(n log n) time, O(n) memory. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* cache[i] = A023359(i) */
    static idx_t max_computed = 0;/* largest i for which cache[i] is valid */

    if (n == 0)                  /* a(0) = 0 by definition */
        return 0;

    /* Allocate or grow the cache if necessary */
    if (cache == NULL) {
        cache = (seq_t *)malloc((n + 1) * sizeof(seq_t));
        if (cache == NULL)
            return 0;                 /* memory allocation failed */
        cache[0] = (seq_t)1;           /* A023359(0) = 1 */
        max_computed = 0;
    } else if (max_computed < n) {
        seq_t *tmp = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (tmp == NULL)
            return 0;                 /* memory allocation failed */
        cache = tmp;
    }

    /* Compute missing DP values up to n */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        seq_t sum = 0;
        for (idx_t k = 1; k <= i; k <<= 1) {
            sum += cache[i - k];
        }
        cache[i] = sum;
    }
    max_computed = n;

    /* a(n) = n * A023359(n) */
    return (seq_t)n * cache[n];
}
