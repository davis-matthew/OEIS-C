/*
 * A384712  Expansion of (1+2*x) / (1-2*x-6*x^2).
 * https://oeis.org/A384712
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, walk
 * Formula:   a(n) = 3*a(n-1) + 4*a(n-2) - 6*a(n-3); a(0)=1, a(1)=4, a(2)=14
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 34, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

#define CACHE_LIMIT 16384

static seq_t cache[CACHE_LIMIT];
static idx_t cache_len = 0;

static void init_cache_upto(idx_t n)
{
    if (cache_len == 0) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)4;
        cache[2] = (seq_t)14;
        cache_len = 3;
    }
    if (n < cache_len) return;
    for (idx_t i = cache_len; i <= n && i < CACHE_LIMIT; ++i) {
        cache[i] = (seq_t)3 * cache[i-1] + (seq_t)4 * cache[i-2] - (seq_t)6 * cache[i-3];
    }
    if (n < CACHE_LIMIT) cache_len = n + 1;
    else cache_len = CACHE_LIMIT;
}

seq_t a(idx_t n)
{
    if (n < CACHE_LIMIT) {
        init_cache_upto(n);
        return cache[n];
    }

    seq_t a0 = (seq_t)1;
    if (n == 0) return a0;
    seq_t a1 = (seq_t)4;
    if (n == 1) return a1;
    seq_t a2 = (seq_t)14;
    if (n == 2) return a2;

    seq_t next;
    for (idx_t i = 3; i <= n; ++i) {
        next = (seq_t)3 * a2 + (seq_t)4 * a1 - (seq_t)6 * a0;
        a0 = a1;
        a1 = a2;
        a2 = next;
    }
    return a2;
}
