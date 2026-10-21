/*
 * A251763  10-step Fibonacci sequence starting with 0,0,0,0,1,0,0,0,0,0.
 * https://oeis.org/A251763
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n+10)=a(n)+a(n+1)+a(n+2)+a(n+3)+a(n+4)+a(n+5)+a(n+6)+a(n+7)+a(n+8)+a(n+9)
 *            a(0)=0, a(1)=0, a(2)=0, a(3)=0, a(4)=1, a(5)=0, a(6)=0, a(7)=0,
 *            a(8)=0, a(9)=0
 * Range:     u8 n <= 18, u16 n <= 26, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache  = NULL;   /* cached sequence values */
    static size_t cache_cap = 0;   /* capacity of the cache array   */
    static idx_t  cache_len = 0;   /* number of values currently stored */

    /* Ensure the cache can hold up to index n. */
    if (cache_cap <= (size_t)n) {
        size_t new_cap = (size_t)n + 1;
        if (new_cap < 10) new_cap = 10;          /* at least 10 for the seed values */
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort(); // allocation failure
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Initialise the first ten terms on first use. */
    if (cache_len == 0) {
        cache[0] = 0;
        cache[1] = 0;
        cache[2] = 0;
        cache[3] = 0;
        cache[4] = 1;
        cache[5] = 0;
        cache[6] = 0;
        cache[7] = 0;
        cache[8] = 0;
        cache[9] = 0;
        cache_len = 10;
    }

    /* Return the cached value if it has already been computed. */
    if (n < cache_len)
        return cache[(size_t)n];

    /* Compute all terms up to the requested index. */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t sum = 0;
        for (idx_t j = i - 10; j < i; ++j)
            sum += cache[(size_t)j];
        cache[(size_t)i] = sum;
    }
    cache_len = n + 1;
    return cache[(size_t)n];
}
