/*
 * A343098  Number of palindromes < 10^n whose squares are also palindromes.
 * https://oeis.org/A343098
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) =
 *            a(n-1)+4*a(n-2)-4*a(n-3)-6*a(n-4)+6*a(n-5)+4*a(n-6)-4*a(n-7)-a(n-8)+a(n-9)
 *            for n>9 Initial values: a(0)=1, a(1)=4, a(2)=6, a(3)=11,
 *            a(4)=14, a(5)=22, a(6)=27, a(7)=40, a(8)=49, a(9)=71
 * Range:     u8 n <= 14, u16 n <= 55, u32 n <= 863, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <assert.h>

static seq_t *cache = NULL;      /* pointer to cached values */
static idx_t cache_cap = 0;      /* capacity of the cache array */
static idx_t cache_len = 0;      /* number of already computed entries */

/* Public function required by the build. */
seq_t a(idx_t n)
{
    /* Initialise cache on first call. */
    if (cache_cap == 0) {
        cache_cap = 10;                 /* we know at least 10 values */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        assert(cache != NULL);
        /* pre-fill the first 10 terms */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)4;
        cache[2] = (seq_t)6;
        cache[3] = (seq_t)11;
        cache[4] = (seq_t)14;
        cache[5] = (seq_t)22;
        cache[6] = (seq_t)27;
        cache[7] = (seq_t)40;
        cache[8] = (seq_t)49;
        cache[9] = (seq_t)71;
        cache_len = 10;
    }

    /* If needed, grow the cache to accommodate n. */
    if (n >= cache_cap) {
        idx_t new_cap = n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        assert(new_cache != NULL);
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* If the requested index is already cached, return it directly. */
    if (n < cache_len)
        return cache[n];

    /* Compute any missing values up to n. */
    for (idx_t i = cache_len; i <= n; ++i) {
        /* The recurrence involves subtraction; use a signed 128‑bit
           intermediate to avoid underflow. */
        __int128 val = 0;
        val += (__int128)cache[i - 1];
        val += (__int128)4 * (__int128)cache[i - 2];
        val -= (__int128)4 * (__int128)cache[i - 3];
        val -= (__int128)6 * (__int128)cache[i - 4];
        val += (__int128)6 * (__int128)cache[i - 5];
        val += (__int128)4 * (__int128)cache[i - 6];
        val -= (__int128)4 * (__int128)cache[i - 7];
        val -= (__int128)cache[i - 8];
        val += (__int128)cache[i - 9];
        cache[i] = (seq_t)val;
    }
    cache_len = n + 1;

    return cache[n];
}
