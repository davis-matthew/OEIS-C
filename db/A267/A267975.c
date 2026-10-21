/*
 * A267975  Number of 1 X n 0..3 arrays with every repeated value in every row
 *          greater than or equal to, and in every column greater than, the
 *          previous repeated value.
 * https://oeis.org/A267975
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 34, u128 n <= 70,
 *            big any n
 * Helpers:   alloc_seq, free_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Empirical recurrence for A267975:
   a(n) = 12*a(n-1) - 50*a(n-2) + 72*a(n-3) + 21*a(n-4)
          - 72*a(n-5) - 50*a(n-6) - 12*a(n-7) - a(n-8)
   with initial values:
   a(1)=4, a(2)=16, a(3)=64, a(4)=250, a(5)=964, a(6)=3674,
   a(7)=13868, a(8)=51917. */
#include <stddef.h>
#include "../utils/alloc_seq.h"
#include "../utils/free_seq.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    if (cache == NULL)
    {
        cache_size = 8;
        cache = alloc_seq(cache_size);
        cache[0] = 4;
        cache[1] = 16;
        cache[2] = 64;
        cache[3] = 250;
        cache[4] = 964;
        cache[5] = 3674;
        cache[6] = 13868;
        cache[7] = 51917;
    }

    if (n <= cache_size)
        return cache[n - 1];

    /* Grow the cache to hold up to index n */
    seq_t *new_cache = alloc_seq(n);
    for (idx_t i = 0; i < cache_size; ++i)
        new_cache[i] = cache[i];

    for (idx_t i = cache_size; i < n; ++i)
    {
        /*
           Compute a(i+1) using only additions and a final subtraction.
           Positive part: 12*a(i) + 72*a(i-2) + 21*a(i-3)
           Negative part: 50*a(i-1) + 72*a(i-3) + 50*a(i-4)
                          + 12*a(i-5) + a(i-6)
        */
        seq_t pos = 12 * new_cache[i - 1] + 72 * new_cache[i - 3] + 21 * new_cache[i - 4];
        seq_t neg = 50 * new_cache[i - 2] + 72 * new_cache[i - 5] + 50 * new_cache[i - 6]
                    + 12 * new_cache[i - 7] + new_cache[i - 8];
        new_cache[i] = pos - neg;
    }

    free_seq(cache);
    cache      = new_cache;
    cache_size = n;

    return new_cache[n - 1];
}
