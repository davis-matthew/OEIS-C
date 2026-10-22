/*
 * A303721  Number of nX2 0..1 arrays with every element unequal to 0, 1, 2 or
 *          3 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A303721
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) + a(n-2) - 4*a(n-3) - 4*a(n-4) (n>5) Initial
 *            values: a(1)=2, a(2)=8, a(3)=21, a(4)=49, a(5)=120 The
 *            implementation memoizes results so that calls with arbitrary
 *            index order are O(n) total for the largest queried n
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 24, u64 n <= 48, u128 n <= 96,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* static cache: index 1..cache_len are stored; cache_cap is allocated capacity */
static seq_t *cache = NULL;
static idx_t  cache_len = 0;
static idx_t  cache_cap = 0;

/* Ensure the cache can hold indices up to n inclusive. */
static void
ensure_capacity(idx_t n)
{
    if (n < cache_cap)
        return;
    idx_t new_cap = cache_cap ? cache_cap << 1 : 8;
    while (new_cap <= n)
        new_cap <<= 1;
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

/* Initialize the first five terms if not already done. */
static void
init_base(void)
{
    if (cache_len >= 5)
        return;
    if (cache_cap < 6) {
        cache = (seq_t *)realloc(cache, 6 * sizeof(seq_t));
        cache_cap = 6;
    }
    cache[1] = (idx_t)2;
    cache[2] = (idx_t)8;
    cache[3] = (idx_t)21;
    cache[4] = (idx_t)49;
    cache[5] = (idx_t)120;
    cache_len = 5;
}

/* Public sequence access function */
seq_t
a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;   /* undefined for n<1, but safe */

    if (n <= cache_len)
        return cache[n];

    ensure_capacity(n + 1);  /* allocate space up to n */
    init_base();             /* set initial values if needed */

    for (idx_t i = cache_len + 1; i <= n; ++i) {
        if (i > 5) {
            cache[i] =
                (seq_t)3 * cache[i - 1] + cache[i - 2]
                - (seq_t)4 * cache[i - 3] - (seq_t)4 * cache[i - 4];
        }
        /* i <= 5: values already initialized */
    }
    cache_len = n;
    return cache[n];
}
