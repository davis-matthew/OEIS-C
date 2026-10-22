/*
 * A377500  Number of edge cuts in the n-dipyramidal graph.
 * https://oeis.org/A377500
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) =
 *            24*a(n-1)-215*a(n-2)+896*a(n-3)-1808*a(n-4)+1760*a(n-5)-784*a(n-6)+128*a(n-7)
 *            Initial values: a(0)=1, a(1)=6, a(2)=32, a(3)=198, a(4)=1440,
 *            a(5)=11606, a(6)=98288
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t cache_len = 0;

static void ensure_capacity(idx_t needed)
{
    if ((size_t)needed < cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap : 8;
    while (new_cap <= (size_t)needed) new_cap <<= 1;
    seq_t *new_arr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = new_arr;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* initialize cache on first call */
    if (cache_len == 0) {
        ensure_capacity(6);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)6;
        cache[2] = (seq_t)32;
        cache[3] = (seq_t)198;
        cache[4] = (seq_t)1440;
        cache[5] = (seq_t)11606;
        cache[6] = (seq_t)98288;
        cache_len = 7;
    }

    if (n < cache_len)
        return cache[n];

    ensure_capacity(n);
    for (idx_t k = cache_len; k <= n; ++k) {
        seq_t pos = 24 * cache[k-1] + 896 * cache[k-3] + 1760 * cache[k-5] + 128 * cache[k-7];
        seq_t neg = 215 * cache[k-2] + 1808 * cache[k-4] + 784 * cache[k-6];
        cache[k] = pos - neg;   /* pos >= neg for all n, so unsigned subtraction is safe */
    }
    cache_len = n + 1;
    return cache[n];
}
