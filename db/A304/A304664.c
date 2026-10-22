/*
 * A304664  Number of nX3 0..1 arrays with every element unequal to 0, 1, 3, 5
 *          or 6 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A304664
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 4*a(n-3) + 2*a(n-5) - a(n-7) - 2*a(n-9) -
 *            2*a(n-11) - a(n-12) initial values: a(1)=3, a(2)=9, a(3)=13,
 *            a(4)=30, a(5)=66, a(6)=123, a(7)=252, a(8)=535, a(9)=1074,
 *            a(10)=2194, a(11)=4530, a(12)=9231
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 30, u64 n <= 61, u128 n <= 123,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

static seq_t *cache = NULL;        /* memoized sequence values */
static idx_t  cache_size = 0;      /* number of valid entries (max index +1) */
static size_t cache_cap  = 0;      /* allocated capacity */

/* Ensure cache has room for indices up to n (inclusive) */
static void ensure_cache(idx_t n)
{
    if (n < cache_size)
        return;                          /* already present */

    size_t need = (size_t)(n + 1);
    if (cache_cap < need) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 16;
        while (new_cap < need)
            new_cap <<= 1;
        seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!new_cache) abort();          /* allocation failure */
        if (cache) {
            memcpy(new_cache, cache, cache_size * sizeof(seq_t));
            free(cache);
        }
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Initialize base values if this is the first time */
    if (cache_size == 0) {
        cache[0] = 0;                     /* unused, sequence starts at 1 */
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)9;
        cache[3] = (seq_t)13;
        cache[4] = (seq_t)30;
        cache[5] = (seq_t)66;
        cache[6] = (seq_t)123;
        cache[7] = (seq_t)252;
        cache[8] = (seq_t)535;
        cache[9] = (seq_t)1074;
        cache[10] = (seq_t)2194;
        cache[11] = (seq_t)4530;
        cache[12] = (seq_t)9231;
        cache_size = 13;
    }

    /* Now compute values up to n using the recurrence */
    for (idx_t i = cache_size; i <= n; ++i) {
        if (i <= 12) {
            /* base cases already set */
            continue;
        }
        seq_t pos = cache[i - 1];
        if (i >= 4) pos += (seq_t)4 * cache[i - 3];
        if (i >= 6) pos += (seq_t)2 * cache[i - 5];

        seq_t neg = 0;
        if (i >= 8) neg += cache[i - 7];
        if (i >= 10) neg += (seq_t)2 * cache[i - 9];
        if (i >= 12) neg += (seq_t)2 * cache[i - 11];
        if (i >= 13) neg += cache[i - 12];

        cache[i] = pos - neg;
    }
    cache_size = n + 1;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined, return 0 */
    ensure_cache(n);
    return cache[n];
}
