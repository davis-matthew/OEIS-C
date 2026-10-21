/*
 * A267928  Number of n X 1 0..3 arrays with every repeated value in every row
 *          and column greater than the previous repeated value.
 * https://oeis.org/A267928
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 12*a(n-1) - 48*a(n-2) + 44*a(n-3) + 150*a(n-4) -
 *            252*a(n-5) - 216*a(n-6) + 324*a(n-7) + 243*a(n-8), with initial
 *            values a(1)=4, a(2)=16, a(3)=60, a(4)=222, a(5)=804, a(6)=2872,
 *            a(7)=10132, a(8)=35383
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 36, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static const seq_t init[8] = {4, 16, 60, 222, 804, 2872, 10132, 35383};

/* Cache for computed values.  Size is large enough for n ≤ 10000. */
#define MAX_N 10000
static seq_t cache[MAX_N + 1];
static size_t cache_len = 0;

/* Ensure the cache is initialized with the first 8 terms. */
static inline void init_cache(void)
{
    if (cache_len != 0)
        return;

    for (idx_t i = 1; i <= 8; ++i)
        cache[i] = init[i - 1];
    cache_len = 8;
}

/* Return a(n) for n ≥ 1.  The function is memoised; calls may be in any order. */
seq_t a(idx_t n)
{
    if (n == 0)    /* unused offset, harmless */
        return 0;

    if (n <= 8)
        return init[(size_t)n - 1];

    init_cache();

    if (n <= (idx_t)cache_len)
        return cache[n];

    for (idx_t i = (idx_t)cache_len + 1; i <= n; ++i)
    {
        cache[i] =
            12 * cache[i - 1] - 48 * cache[i - 2] + 44 * cache[i - 3] +
            150 * cache[i - 4] - 252 * cache[i - 5] - 216 * cache[i - 6] +
            324 * cache[i - 7] + 243 * cache[i - 8];
    }
    cache_len = (size_t)n;
    return cache[n];
}
