/*
 * A358500  a(n) = Sum_{k=0..floor(n/5)} (n-5*k)!.
 * https://oeis.org/A358500
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..floor(n/5)} (n-5k)! Recurrence: a(n) = n*a(n-1)
 *            + a(n-5) - n*a(n-6) for n>5 Initial values: a(0)=1, a(1)=1,
 *            a(2)=2, a(3)=6, a(4)=24, a(5)=121
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* Cached sequence values */
static size_t cache_cap = 0;     /* Allocated number of elements */

seq_t a(idx_t n)
{
    /* Ensure the cache holds entries up to index n. */
    size_t target = n + 1;
    if (target > cache_cap) {
        /* At least 6 slots are needed for the initial values. */
        size_t new_cap = target < 6 ? 6 : target;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;

        /* First time: initialise the first six terms. */
        if (cache_cap == 0) {
            cache[0] = (seq_t)1;  /* sum of 0! */
            cache[1] = (seq_t)1;  /* 0! + 1! */
            cache[2] = (seq_t)2;  /* 0! + 1! + 2! */
            cache[3] = (seq_t)6;  /* 0! + 1! + 2! + 3! */
            cache[4] = (seq_t)24; /* up to 4! */
            cache[5] = (seq_t)121;/* up to 5! + 0! */
            cache_cap = 6;
        }

        /* Compute any new entries using the recurrence. */
        for (size_t i = cache_cap; i < new_cap; ++i) {
            if (i <= 5) continue;   /* already set */

            seq_t t1 = (seq_t)i * cache[i - 1];
            seq_t t2 = cache[i - 5];
            seq_t t3 = (seq_t)i * cache[i - 6];
            cache[i] = t1 + t2 - t3;
        }
        cache_cap = new_cap;
    }
    return cache[n];
}
