/*
 * A247595  a(n) = 4*a(n-1) - 4*a(n-2) + 4*a(n-3) with a(0) = 1, a(1) = 3,
 *          a(2) = 10.
 * https://oeis.org/A247595
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) - 4*a(n-2) + 4*a(n-3) with a(0)=1, a(1)=3,
 *            a(2)=10
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 38, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* memoization table for sequence values   */
static size_t cache_len = 0;     /* number of elements currently stored     */
static size_t cache_cap = 0;     /* allocated capacity of the cache         */

/* Helper: expand cache capacity if needed (grow by doubling) */
static void ensure_capacity(idx_t n)
{
    size_t required = (size_t)n + 1;
    if (required <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 16;
    while (new_cap < required)
        new_cap <<= 1;
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

/* Initialize first three terms if the cache is empty or incomplete */
static void initialize_base(void)
{
    if (cache_len == 0) {
        ensure_capacity(2);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)10;
        cache_len = 3;
    } else {
        if (cache_len < 3) {
            if (cache_len == 1) {
                cache[1] = (seq_t)3;
                cache[2] = (seq_t)10;
                cache_len = 3;
            } else if (cache_len == 2) {
                cache[2] = (seq_t)10;
                cache_len = 3;
            }
        }
    }
}

seq_t a(idx_t n)
{
    if (n < 0)  /* undefined, but just return 0 for safety */
        return (seq_t)0;

    ensure_capacity(n);
    initialize_base();

    if ((size_t)n < cache_len)
        return cache[(size_t)n];

    /* compute missing terms up to n */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        seq_t t = cache[i-1] + cache[i-3];   /* a_{i-1} + a_{i-3} */
        t -= cache[i-2];                    /* minus a_{i-2} */
        t *= (seq_t)4;                      /* multiply by 4 */
        cache[i] = t;
    }
    cache_len = (size_t)n + 1;
    return cache[(size_t)n];
}
