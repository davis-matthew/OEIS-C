/*
 * A252701  Number of strings of length n over an 8-letter alphabet that do
 *          not begin with a palindrome.
 * https://oeis.org/A252701
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, walk
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:  a(0) = 0, a(1) = 8,
 *               a(n) = 8 * a(n-1) - a[(n+1)/2]   for n ≥ 2
 *
 * This implements OEIS A252701: number of strings of length n over
 * an 8‑letter alphabet that do not begin with a palindrome.  The
 * recurrence follows from
 *
 *     b(n) = 8 * b(n-1) + a[(n+1)/2]   with  b(n) = 8ⁿ – a(n),
 *     b(0) = 1 , b(1) = 0
 *
 * eliminating b(n) gives the above simple recurrence.  All
 * computations are performed with seq_t and idx_t supplied by the
 * build system.  The cache is lazy‑initialised and grown only when
 * required; the function may be called in any order of indices.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for previously computed values */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;  /* number of values stored */

    /* initialise cache for a(0) and a(1) if not already done */
    if (cache_len == 0) {
        cache_len = 2;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        cache[0] = 0;
        cache[1] = 8;
    }

    /* extend cache up to index n, if necessary */
    if (n >= cache_len) {
        idx_t new_len = n + 1;
        cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        for (idx_t i = cache_len; i <= n; ++i) {
            if (i == 0) { cache[0] = 0; }
            else if (i == 1) { cache[1] = 8; }
            else {
                idx_t mid = (i + 1) / 2;       /* ceil(i/2) */
                cache[i] = (seq_t)8 * cache[i-1] - cache[mid];
            }
        }
        cache_len = new_len;
    }

    return cache[n];
}
