/*
 * A341579  Number of steps needed to solve the Towers of Hanoi exchanging
 *          disks puzzle with 3 pegs and n disks.
 * https://oeis.org/A341579
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + 2*a(n-4) + 3 (n≥4) Initial values:
 *            a(0)=0, a(1)=1, a(2)=3, a(3)=7
 * Range:     u8 n <= 8, u16 n <= 17, u32 n <= 35, u64 n <= 71, u128 n <= 143,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_cap = 0;
static idx_t cache_len = 0; /* number of values currently computed (0..cache_len-1) */

static void ensure_cache(idx_t n)
{
    if (n < cache_len)
        return;

    /* grow storage */
    if (cache_cap == 0) {
        cache_cap = 64;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
    }
    while (cache_cap <= n) {
        cache_cap <<= 1; /* double */
        cache = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
    }

    /* compute missing terms */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i == 0)
            cache[i] = (seq_t)0;
        else if (i == 1)
            cache[i] = (seq_t)1;
        else if (i == 2)
            cache[i] = (seq_t)3;
        else if (i == 3)
            cache[i] = (seq_t)7;
        else
            cache[i] = cache[i-1] + cache[i-2] + ((seq_t)2 * cache[i-4]) + (seq_t)3;
    }

    cache_len = n + 1;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
