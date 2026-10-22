/*
 * A301671  Partial sums of A301670.
 * https://oeis.org/A301671
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-2)+a(n-3)+a(n-4)-a(n-5)-a(n-6)-a(n-7)+a(n-9), a(0)=1,
 *            a(1)=5, a(2)=13, a(3)=25, a(4)=41, a(5)=63, a(6)=89, a(7)=115,
 *            a(8)=151, a(9)=187, 0‑based offset
 * Range:     u8 n <= 10, u16 n <= 176, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

static const seq_t _init[10] = {
    (seq_t)1, (seq_t)5, (seq_t)13, (seq_t)25, (seq_t)41,
    (seq_t)63, (seq_t)89, (seq_t)115, (seq_t)151, (seq_t)187
};

seq_t a(idx_t n)
{
    /* static dynamic cache */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* allocated capacity */
    static size_t cache_len = 0;   /* number of filled entries */

    /* Initialise cache with first 10 terms */
    if (!cache) {
        cache_cap = 10;
        cache = alloc_seq_array(cache_cap);
        for (size_t i = 0; i < 10; ++i)
            cache[i] = _init[i];
        cache_len = 10;
    }

    /* Grow cache to at least n+1 entries if needed */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = (size_t)n + 1;
        if (new_cap < cache_cap * 2)
            new_cap = cache_cap * 2;
        seq_t *tmp = alloc_seq_array(new_cap);
        for (size_t i = 0; i < cache_len; ++i)
            tmp[i] = cache[i];
        free_seq_array(cache);
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing entries up to n */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        if (i < 10) {  /* values already initialized */
            continue;
        }
        seq_t sum = cache[i - 2] + cache[i - 3] + cache[i - 4]
                  - cache[i - 5] - cache[i - 6] - cache[i - 7]
                  + cache[i - 9];
        cache[i] = sum;
    }
    if ((size_t)n + 1 > cache_len)
        cache_len = (size_t)n + 1;

    return cache[(size_t)n];
}
