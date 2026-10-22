/*
 * A301290  Partial sums of A301289.
 * https://oeis.org/A301290
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) - 5*a(n-2) + 7*a(n-3) - 8*a(n-4) * + 8*a(n-5) -
 *            7*a(n-6) + 5*a(n-7) - 3*a(n-8) + a(n-9) * with a(0)=1, a(1)=5,
 *            a(2)=10, a(3)=16, a(4)=28, * a(5)=42, a(6)=57, a(7)=75, a(8)=96,
 *            a(9)=122
 * Range:     u8 n <= 13, u16 n <= 221, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

static const seq_t init_vals[10] = {
    1, 5, 10, 16, 28, 42, 57, 75, 96, 122
};

/* A static cache large enough for the expected range (≈10000). */
static seq_t cache[65536];
static idx_t cache_len = 0;

/* Initialise the cache with the first 10 terms. */
static void init_cache(void)
{
    if (cache_len == 0) {
        memcpy(cache, init_vals, sizeof(init_vals));
        cache_len = 10;
    }
}

/* Compute the nth term of the sequence. */
seq_t a(idx_t n)
{
    init_cache();

    if (n < cache_len)
        return cache[n];

    /* Compute terms from cache_len up to n, inclusive. */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t v = 0;
        v += cache[i - 1] * (seq_t)3;
        v -= cache[i - 2] * (seq_t)5;
        v += cache[i - 3] * (seq_t)7;
        v -= cache[i - 4] * (seq_t)8;
        v += cache[i - 5] * (seq_t)8;
        v -= cache[i - 6] * (seq_t)7;
        v += cache[i - 7] * (seq_t)5;
        v -= cache[i - 8] * (seq_t)3;
        v += cache[i - 9];
        cache[i] = v;
    }

    cache_len = n + 1;
    return cache[n];
}
