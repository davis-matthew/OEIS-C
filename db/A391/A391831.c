/*
 * A391831  a(n) = (1/4) * Sum_{k=0..floor(n/3)} (n-2*k+2) *
 *          binomial(2*n-4*k+2,2*k+1).
 * https://oeis.org/A391831
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 26, u64 n <= 54, u128 n <= 111,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: initial terms: a(0)=1, a(1)=3, a(2)=6, a(3)=13, a(4)=35, a(5)=91,
   a(6)=214, a(7)=491, a(8)=1151, a(9)=2711, a(10)=6290, a(11)=14427;
   For n ≥ 12:
     a(n) = 4*a(n-1) - 6*a(n-2) + 8*a(n-3) - 5*a(n-4) - 4*a(n-5)
            - 2*a(n-6) - 4*a(n-7) - 6*a(n-8) + 4*a(n-9) + 4*a(n-10)
            - a(n-12) */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/mul_small.h"

static seq_t *cache = NULL;
static size_t cache_size = 0;

/* Initial values for indices 0..11 */
static const seq_t init_vals[12] = {
    (seq_t)1,  (seq_t)3,  (seq_t)6,   (seq_t)13, (seq_t)35,
    (seq_t)91, (seq_t)214, (seq_t)491, (seq_t)1151, (seq_t)2711,
    (seq_t)6290, (seq_t)14427
};

static void ensure_cache(idx_t n)
{
    if (cache == NULL) {
        /* Allocate initial 12 entries */
        cache_size = 12;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        for (size_t i = 0; i < 12; ++i)
            cache[i] = init_vals[i];
    }

    if ((idx_t)cache_size > n)
        return; /* already computed up to n */

    /* Expand cache to accommodate index n */
    size_t old_size = cache_size;
    size_t new_size = (size_t)n + 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!new_cache)
        return; /* on failure keep old cache; caller will handle result */

    cache = new_cache;
    cache_size = new_size;

    /* Compute missing terms using the recurrence */
    for (idx_t i = (idx_t)old_size; i <= n; ++i) {
        seq_t v = mul_small(cache[i - 1], 4);
        v -= mul_small(cache[i - 2], 6);
        v += mul_small(cache[i - 3], 8);
        v -= mul_small(cache[i - 4], 5);
        v -= mul_small(cache[i - 5], 4);
        v -= mul_small(cache[i - 6], 2);
        v -= mul_small(cache[i - 7], 4);
        v -= mul_small(cache[i - 8], 6);
        v += mul_small(cache[i - 9], 4);
        v += mul_small(cache[i - 10], 4);
        v -= cache[i - 12];
        cache[i] = v;
    }
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
