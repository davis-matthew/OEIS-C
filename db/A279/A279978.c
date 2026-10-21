/*
 * A279978  Number of 2 X n 0..1 arrays with no element equal to a strict
 *          majority of its horizontal and antidiagonal neighbors, with the
 *          exception of exactly two elements, and with new values introduced
 *          in order 0 sequentially upwards.
 * https://oeis.org/A279978
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-1) - 7*a(n-2) - 2*a(n-3) + 10*a(n-4) - 2*a(n-5) -
 *            5*a(n-6) + a(n-7) + a(n-8) Initial values: a(1)=0, a(2)=3,
 *            a(3)=9, a(4)=24, a(5)=62, a(6)=134, a(7)=277, a(8)=542,
 *            a(9)=1035, a(10)=1930
 * Range:     u8 n <= 6, u16 n <= 16, u32 n <= 36, u64 n <= 80, u128 n <= 169,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init_vals[10] = {
    0, 3, 9, 24, 62, 134, 277, 542, 1035, 1930
};

static seq_t *cache = NULL;      /* cached sequence values */
static size_t cache_len = 0;     /* number of values in cache */

static void init_cache(void)
{
    cache_len = 10;
    cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
    for (size_t i = 0; i < 10; ++i)
        cache[i] = init_vals[i];
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* undefined for n<1, return 0 as a safe default */

    if (!cache)
        init_cache();

    if (n <= cache_len)
        return cache[n - 1];

    /* Extend cache up to n */
    size_t new_len = n;
    cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));

    for (size_t i = cache_len; i < n; ++i) {
        /* i corresponds to a(i+1) (1‑based) */
        seq_t a1 = cache[i - 1];
        seq_t a2 = cache[i - 2];
        seq_t a3 = cache[i - 3];
        seq_t a4 = cache[i - 4];
        seq_t a5 = cache[i - 5];
        seq_t a6 = cache[i - 6];
        seq_t a7 = cache[i - 7];
        seq_t a8 = cache[i - 8];
        cache[i] = (seq_t)5 * a1 - (seq_t)7 * a2 - (seq_t)2 * a3
                 + (seq_t)10 * a4 - (seq_t)2 * a5 - (seq_t)5 * a6
                 + a7 + a8;
    }
    cache_len = new_len;
    return cache[n - 1];
}
