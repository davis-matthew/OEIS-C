/*
 * A368852  a(n) = (n!)^3 * Sum_{k=0..n} (-1)^k/(k!)^3.
 * https://oeis.org/A368852
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^3*a(n-1) + (-1)^n; a(0) = 1
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* for realloc, abort */

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* memoisation buffer       */
    static idx_t cache_len = 0;   /* buffer length (in terms) */

    /* Grow cached values until we have a(n) ready. */
    while (cache_len <= n) {
        /* Determine new buffer size – double until large enough. */
        size_t old_len = cache_len;
        size_t new_len = cache_len ? cache_len * 2 : 1;
        while (new_len <= n) new_len <<= 1;
        new_len = n + 1;                       /* must hold a(n)   */

        cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_len);
        if (!cache) abort();                   /* memory error     */
        cache_len = new_len;

        /* Initialise the very first element if this is the first run. */
        if (old_len == 0) {
            cache[0] = (seq_t)1;
            old_len = 1;
        }

        /* Compute all newly required terms (from old_len to n). */
        for (idx_t i = old_len; i <= n; ++i) {
            seq_t prod = cache[i - 1];
            prod = prod * (seq_t)i * (seq_t)i * (seq_t)i; /* n^3 * a(i-1)   */
            if (i & 1)  /* odd i → subtract 1 */
                prod -= (seq_t)1;
            else        /* even i → add 1    */
                prod += (seq_t)1;
            cache[i] = prod;
        }
    }

    return cache[n];
}
