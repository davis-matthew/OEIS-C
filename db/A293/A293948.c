/*
 * A293948  Arises in a coin-weighing problem.
 * https://oeis.org/A293948
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence implementation of OEIS A293948.
   For n < 10 the values are hard‑coded, for n >= 10:
   a[n] = 2 a[n-1] +5 a[n-2] -10 a[n-3] -8 a[n-4] +16 a[n-5] +4 a[n-6] -8 a[n-7]  */
#include <stdlib.h>
#include <stddef.h>

static seq_t *cache = NULL;
static size_t cache_size = 0;
static size_t cache_cap = 0;

static const seq_t init_values[10] = {
    (seq_t)1, (seq_t)1, (seq_t)3, (seq_t)5,
    (seq_t)11, (seq_t)20, (seq_t)41, (seq_t)82,
    (seq_t)163, (seq_t)334
};

static void ensure_cache(idx_t n)
{
    if (cache_size > 0 && n < cache_size)
        return;
    if (cache == NULL) {
        cache_cap = 10;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) return;  /* allocation failed */
        for (size_t i = 0; i < 10; ++i)
            cache[i] = init_values[i];
        cache_size = 10;
    }
    if (n + 1 > cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap <= n) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp != NULL) {
            cache = tmp;
            cache_cap = new_cap;
        }
    }
    for (size_t i = cache_size; i <= (size_t)n; ++i) {
        seq_t next =  (seq_t)2 * cache[i - 1]
                    + (seq_t)5 * cache[i - 2]
                    + (seq_t)16 * cache[i - 5]
                    + (seq_t)4 * cache[i - 6];
        next -= (seq_t)10 * cache[i - 3];
        next -= (seq_t)8 * cache[i - 4];
        next -= (seq_t)8 * cache[i - 7];
        cache[i] = next;
    }
    cache_size = (size_t)n + 1;
}

seq_t a(idx_t n)
{
    if (n < 10)
        return init_values[(size_t)n];
    ensure_cache(n);
    return cache[(size_t)n];
}
