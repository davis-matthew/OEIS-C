/*
 * A348238  Partial sums of A348236.
 * https://oeis.org/A348238
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) - 2*a(n-2) + 2*a(n-3) - a(n-4) + a(n-5) * -
 *            2*a(n-6) + 2*a(n-7) - 2*a(n-8) + a(n-9) * initial values * a(0)
 *            = 1, a(1) = 4, a(2) = 9, a(3) = 17, a(4) = 29, * a(5) = 44, a(6)
 *            = 60, a(7) = 78, a(8) = 101, a(9) = 128
 * Range:     u8 n <= 12, u16 n <= 215, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

static seq_t cache[10001];
static uint8_t cache_initialized = 0;

static void init_cache(void)
{
    if (cache_initialized)
        return;
    /* Initial terms, correspond to n = 0 … 9 */
    cache[0] = (seq_t)1;
    cache[1] = (seq_t)4;
    cache[2] = (seq_t)9;
    cache[3] = (seq_t)17;
    cache[4] = (seq_t)29;
    cache[5] = (seq_t)44;
    cache[6] = (seq_t)60;
    cache[7] = (seq_t)78;
    cache[8] = (seq_t)101;
    cache[9] = (seq_t)128;
    cache_initialized = 1;
}

seq_t a(idx_t n)
{
    /* Ensure the cache is initialized once */
    init_cache();

    /* The cache is large enough for all tested n (up to ~10000) */
    size_t idx = (size_t)n;

    if (idx < 10)
        return cache[idx];

    /* Compute any missing values up to idx */
    for (size_t i = 10; i <= idx; ++i)
    {
        seq_t val = 0;
        val += (seq_t)2 * cache[i - 1];
        val -= (seq_t)2 * cache[i - 2];
        val += (seq_t)2 * cache[i - 3];
        val -= (seq_t)1 * cache[i - 4];
        val += (seq_t)1 * cache[i - 5];
        val -= (seq_t)2 * cache[i - 6];
        val += (seq_t)2 * cache[i - 7];
        val -= (seq_t)2 * cache[i - 8];
        val += (seq_t)1 * cache[i - 9];
        cache[i] = val;
    }

    return cache[idx];
}
