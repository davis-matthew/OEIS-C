/*
 * A292532  p-INVERT of the squares (A000290), where p(S) = 1 - S^3.
 * https://oeis.org/A292532
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9*a(n-1) - 36*a(n-2) + 85*a(n-3) - 123*a(n-4) +
 *            129*a(n-5) - 83*a(n-6) + 36*a(n-7) - 9*a(n-8) + a(n-9) Initial
 *            values (n=0..9): a(0)=0, a(1)=0, a(2)=1, a(3)=12, a(4)=75,
 *            a(5)=329, a(6)=1158, a(7)=3606, a(8)=10971, a(9)=35601
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 18, u64 n <= 35, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t initial_values[10] = {
    (seq_t)0, (seq_t)0, (seq_t)1, (seq_t)12, (seq_t)75,
    (seq_t)329, (seq_t)1158, (seq_t)3606, (seq_t)10971, (seq_t)35601
};

static seq_t *cache = NULL;      /* array of computed terms */
static idx_t  cache_size = 0;    /* number of terms stored (>=10 after first init) */
static idx_t  cache_cap  = 0;    /* allocated capacity */

seq_t a(idx_t n)
{
    /* Return initial values directly for n < 10 */
    if (n < 10) {
        return initial_values[n];
    }

    /* First time: create cache and fill first 10 terms */
    if (cache_size == 0) {
        cache_cap  = 64;            /* initial capacity > 10 */
        cache      = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();       /* out of memory */
        for (idx_t i = 0; i < 10; ++i) {
            cache[i] = initial_values[i];
        }
        cache_size = 10;
    }

    /* Grow cache if necessary */
    if (n >= cache_cap) {
        idx_t new_cap = cache_cap ? cache_cap : 64;
        while (new_cap <= n) new_cap <<= 1;
        seq_t *new_arr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_arr) abort();
        cache = new_arr;
        cache_cap = new_cap;
    }

    /* Compute terms from cache_size up to n */
    for (idx_t i = cache_size; i <= n; ++i) {
        /* We need a[i-1]..a[i-9] which are already computed */
        seq_t val = 9 * cache[i - 1];
        val += 85 * cache[i - 3];
        val += 129 * cache[i - 5];
        val += 36 * cache[i - 7];
        val += 1 * cache[i - 9];

        val -= 36 * cache[i - 2];
        val -= 123 * cache[i - 4];
        val -= 83 * cache[i - 6];
        val -= 9 * cache[i - 8];

        cache[i] = val;
    }
    cache_size = n + 1;   /* we have filled up to n */

    return cache[n];
}
