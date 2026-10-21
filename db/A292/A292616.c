/*
 * A292616  a(n) = 3*a(n-2) - a(n-4) for n > 3, with a(0)=4, a(1)=3,
 *          a(2)=a(3)=6, a sequence related to bisections of Fibonacci
 *          numbers.
 * https://oeis.org/A292616
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-2) - a(n-4) for n>3, a(0)=4, a(1)=3, a(2)=a(3)=6
 * Range:     u8 n <= 10, u16 n <= 21, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* number of elements currently stored */

    /* Return immediately if the value is already cached. */
    if (n < cache_len)
        return cache[n];

    /* Grow the cache to accommodate index n. */
    idx_t old_len = cache_len;
    cache_len   = n + 1;

    /* realloc returns void*, cast to seq_t* so the code compiles in C++ */
    seq_t *tmp = (seq_t *)realloc(cache, (size_t)cache_len * sizeof(seq_t));
    if (tmp == NULL)
        abort();               /* memory allocation failed */
    cache = tmp;

    /* Compute new entries up to n inclusive. */
    for (idx_t i = old_len; i <= n; ++i) {
        if (i == 0) {
            cache[i] = (seq_t)4;
        } else if (i == 1) {
            cache[i] = (seq_t)3;
        } else if (i == 2 || i == 3) {
            cache[i] = (seq_t)6;
        } else {
            cache[i] = 3 * cache[i - 2] - cache[i - 4];
        }
    }

    return cache[n];
}
