/*
 * A256310  Number of partitions of 2n into exactly 6 parts.
 * https://oeis.org/A256310
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of 2n into exactly 6 parts.
 *            Recurrence: a(n) = 2*a(n-1) - 3*a(n-4) + a(n-5) + a(n-6) +
 *            a(n-9) + a(n-10) - 3*a(n-11) + 2*a(n-14) - a(n-15) Initial
 *            values:
 *            a(0)=0,a(1)=0,a(2)=0,a(3)=1,a(4)=2,a(5)=5,a(6)=11,a(7)=20,
 *            a(8)=35,a(9)=58,a(10)=90,a(11)=136,a(12)=199, a(13)=282,...
 * Range:     u8 n <= 12, u16 n <= 42, u32 n <= 407, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* cache[n] holds a(n) */
static idx_t cache_len = 0;   /* number of elements currently stored */

/* Helper: compute a(i) for i >= 16 using the recurrence */
static void fill_range(idx_t start, idx_t end)
{
    for (idx_t i = start; i <= end; ++i) {
        seq_t val = 0;
        val += (seq_t)2 * cache[i-1];
        val -= (seq_t)3 * cache[i-4];
        val += cache[i-5];
        val += cache[i-6];
        val += cache[i-9];
        val += cache[i-10];
        val -= (seq_t)3 * cache[i-11];
        val += (seq_t)2 * cache[i-14];
        val -= cache[i-15];
        cache[i] = val;
    }
}

/* Ensure that cache contains values up to index n. */
static void ensure_cache(idx_t n)
{
    if (cache == NULL) {
        /* First allocation: make space for the requested index and the
           first 16 known terms. */
        size_t init_cap = (n < 16) ? 16 : (size_t)(n + 1);
        cache = (seq_t *)malloc(init_cap * sizeof(seq_t));
        if (!cache) abort();  /* out of memory */

        /* Base values for 0..15 */
        static const seq_t base[16] = {
            0, 0, 0, 1, 2, 5, 11, 20, 35, 58, 90, 136, 199, 282, 391, 532
        };
        for (size_t i = 0; i < 16; ++i) cache[i] = base[i];
        cache_len = 16;

        /* Compute any additional terms required to reach n. */
        if (init_cap > 16) fill_range(16, (idx_t)(init_cap - 1));
        cache_len = init_cap;
    } else if (n >= cache_len) {
        /* Grow the cache to hold up to n. */
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        /* Compute the new range */
        fill_range(cache_len, n);
        cache_len = new_cap;
    }
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
