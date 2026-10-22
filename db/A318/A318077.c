/*
 * A318077  Number of nX3 0..1 arrays with every element unequal to 0, 1, 3 or
 *          5 horizontally, vertically or antidiagonally adjacent elements,
 *          with upper left element zero.
 * https://oeis.org/A318077
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 19, u32 n <= 37, u64 n <= 75, u128 n <= 149,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = a(n-1) + 8*a(n-4) - 2*a(n-5) - 24*a(n-8) - 3*a(n-9)
 *            + 34*a(n-12) + 4*a(n-13) - 24*a(n-16) - a(n-17) + 8*a(n-20)
 *            - a(n-24)  (valid for n >= 25)
 * Initial values: a(1)=3, a(2)=4, a(3)=5, a(4)=11, a(5)=18, a(6)=32,
 *                 a(7)=54, a(8)=98, a(9)=173, a(10)=315, a(11)=559,
 *                 a(12)=1008, a(13)=1813, a(14)=3290, a(15)=5928,
 *                 a(16)=10716, a(17)=19352, a(18)=35036, a(19)=63355,
 *                 a(20)=114651, a(21)=207352, a(22)=375184, a(23)=678789,
 *                 a(24)=1228507
 */

#include <stdlib.h>

static const seq_t init_vals[25] = {
    0,          /* dummy value for index 0 */
    3, 4, 5, 11, 18, 32, 54, 98, 173, 315, 559,
    1008, 1813, 3290, 5928, 10716, 19352, 35036, 63355,
    114651, 207352, 375184, 678789, 1228507
};

static seq_t *cache = NULL;
static idx_t cache_cap = 0;

/* Ensure that the cache can hold values up to index n (inclusive). */
static void ensure_cache(idx_t n)
{
    if (n <= cache_cap)
        return;
    idx_t new_cap = cache_cap * 2 + 10;
    if (new_cap < n)
        new_cap = n;
    /* reallocate and, if large, initialize new entries to zero. */
    seq_t *tmp = (seq_t *)realloc(cache, (new_cap + 1) * sizeof(seq_t));
    if (!tmp)
        abort();                      /* allocation failed */
    cache = tmp;
    /* initialise new positions to zero */
    for (idx_t i = cache_cap + 1; i <= new_cap; ++i)
        cache[i] = 0;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                     /* safety for n < 1 */

    ensure_cache(n);

    for (idx_t i = 1; i <= n; ++i) {
        if (i <= 24) {                /* values from the table */
            if (cache[i] == 0)
                cache[i] = init_vals[i];
            continue;
        }
        if (cache[i] != 0)           /* already computed */
            continue;

        /* compute a(i) via the recurrence (all indices >= 1) */
        seq_t val = cache[i - 1];
        val += (seq_t)8 * cache[i - 4];
        val -= (seq_t)2 * cache[i - 5];
        val -= (seq_t)24 * cache[i - 8];
        val -= (seq_t)3 * cache[i - 9];
        val += (seq_t)34 * cache[i - 12];
        val += (seq_t)4 * cache[i - 13];
        val -= (seq_t)24 * cache[i - 16];
        val -= cache[i - 17];
        val += (seq_t)8 * cache[i - 20];
        val -= cache[i - 24];

        cache[i] = val;
    }
    return cache[n];
}
