/*
 * A259292  Number of (n+1) X (3+1) 0..1 arrays with each 2 X 2 subblock
 *          having clockwise pattern 0001 0101 or 0111.
 * https://oeis.org/A259292
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + 18*a(n-2) - 13*a(n-3) - 70*a(n-4) + 24*a(n-5)
 *            + 64*a(n-6) for n ≥ 7 Initial terms: a(1)=66, a(2)=322,
 *            a(3)=1460, a(4)=6870, a(5)=31774, a(6)=148296
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 12, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

static seq_t *cache = NULL;
static idx_t cache_cap = 0;
static idx_t cache_len = 0;
static bool initialized = false;

static void ensure_cache(idx_t n)
{
    if (cache_cap >= n)
        return;

    idx_t new_cap = cache_cap ? cache_cap : 16;
    while (new_cap < n)
        new_cap <<= 1;

    seq_t *new_cache = (seq_t *)realloc(cache, sizeof(seq_t) * (new_cap + 1));
    if (!new_cache) /* out of memory; abort silently */
        return;
    cache = new_cache;
    /* new entries are implicitly zero-initialized by realloc for new space */
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n < (idx_t)1)
        return (seq_t)0;

    ensure_cache(n);

    if (!initialized) {
        cache[1] = (seq_t)66;
        cache[2] = (seq_t)322;
        cache[3] = (seq_t)1460;
        cache[4] = (seq_t)6870;
        cache[5] = (seq_t)31774;
        cache[6] = (seq_t)148296;
        cache_len = 6;
        initialized = true;
    }

    if (cache_len < n) {
        for (idx_t i = cache_len + 1; i <= n; ++i) {
            seq_t pos = 2 * cache[i - 1] + 18 * cache[i - 2]
                      + 24 * cache[i - 5] + 64 * cache[i - 6];
            seq_t neg = 13 * cache[i - 3] + 70 * cache[i - 4];
            cache[i] = pos - neg;
        }
        cache_len = n;
    }

    return cache[n];
}
