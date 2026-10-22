/*
 * A397793  Number of ways to tile a 2 X n strip with both bottom corners
 *          missing, using squares, dominoes, and Z-shaped tetrominoes.
 * https://oeis.org/A397793
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + 6*a(n-2) - 2*a(n-3) - 4*a(n-4) + a(n-6),
 *            a(0)=0, a(1)=0, a(2)=2, a(3)=4, a(4)=19, a(5)=58, a(6)=214
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 19, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

static seq_t *cache = NULL;
static idx_t cache_size = 0;
static idx_t max_computed = 0;
static bool initialized = false;

static void ensure_capacity(idx_t needed)
{
    if (needed < cache_size) return;
    idx_t new_size = needed + 1;
    seq_t *new_cache = (seq_t*)calloc(new_size, sizeof(seq_t));
    if (cache) {
        memcpy(new_cache, cache, cache_size * sizeof(seq_t));
        free(cache);
    }
    cache = new_cache;
    cache_size = new_size;
}

seq_t a(idx_t n)
{
    if (!initialized) {
        ensure_capacity(6);
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)2;
        cache[3] = (seq_t)4;
        cache[4] = (seq_t)19;
        cache[5] = (seq_t)58;
        cache[6] = (seq_t)214;
        max_computed = 6;
        initialized = true;
    }

    if (n < 2) return (seq_t)0;

    if (n > max_computed) {
        ensure_capacity(n);
        for (idx_t i = max_computed + 1; i <= n; ++i) {
            seq_t pos = 2*cache[i-1] + 6*cache[i-2] + cache[i-6];
            seq_t neg = 2*cache[i-3] + 4*cache[i-4];
            cache[i] = pos - neg;
        }
        max_computed = n;
    }

    return cache[n];
}
