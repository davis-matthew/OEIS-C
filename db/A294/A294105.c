/*
 * A294105  Number of compositions (ordered partitions) of n into squares
 *          dividing n.
 * https://oeis.org/A294105
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of compositions of n into squares that divide n
 *            offset 0 (a(0)=1)
 * Range:     u8 n <= 19, u16 n <= 35, u32 n <= 71, u64 n <= 139,
 *            u128 n <= 279, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* static cache for memoisation of final values */
static seq_t *cache = NULL;
static idx_t  cache_size = 0;   /* number of allocated indices */

/* Ensure the cache can hold indices up to n (inclusive). */
static void ensure_cache(idx_t n)
{
    if (n < cache_size)
        return;                  /* already large enough */

    idx_t new_size = cache_size ? cache_size : 1;
    while (new_size <= n)
        new_size <<= 1;          /* double until big enough */

    seq_t *new_cache = (seq_t*)calloc(new_size, sizeof(seq_t));
    if (!new_cache)
        abort();                /* allocation failed */

    if (cache) {
        for (idx_t i = 0; i < cache_size; ++i)
            new_cache[i] = cache[i];
        free(cache);            /* free old allocation */
    }

    cache      = new_cache;
    cache_size = new_size;
}

/* Return the number of compositions (ordered partitions) of n into
   squares that divide n, for any n ≥ 0. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;         /* a(0) = 1, no memoisation needed */

    /* return cached value if already computed */
    if (n < cache_size && cache[n] != 0)
        return cache[n];

    /* Find all square divisors of n: s^2 that divide n (s ≥ 1). */
    /* Use a small statically sized array: maximum number of square
       divisors for n ≤ 10000 is < 128. */
    idx_t squares[128];
    size_t sq_cnt = 0;
    for (idx_t s = 1; s <= n / s; ++s) {
        idx_t sq = s * s;
        if (n % sq == 0)
            squares[sq_cnt++] = sq;
    }

    /* Dynamic programming for compositions using the squares list. */
    seq_t *dp = (seq_t*)calloc((size_t)n + 1, sizeof(seq_t));
    if (!dp)
        abort();                 /* allocation failed */
    dp[0] = (seq_t)1;

    for (idx_t i = 1; i <= n; ++i) {
        seq_t total = 0;
        for (size_t k = 0; k < sq_cnt; ++k) {
            idx_t part = squares[k];
            if (part <= i)
                total += dp[i - part];
        }
        dp[i] = total;
    }

    seq_t result = dp[n];
    free(dp);

    /* Store in cache for future calls. */
    ensure_cache(n);
    cache[n] = result;

    return result;
}
