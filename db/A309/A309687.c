/*
 * A309687  Number of odd parts appearing among the second largest parts of
 *          the partitions of n into 3 parts.
 * https://oeis.org/A309687
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{j=1..floor(n/3)} Sum_{i=j..floor((n-j)/2)} (i mod 2)
 *            Recurrence: a(0..9) = 0,0,0,1,1,1,1,2,3,4 a(n) = 2*a(n-1) -
 *            2*a(n-2) + 2*a(n-3) - a(n-4) + a(n-6) - 2*a(n-7) + 2*a(n-8) -
 *            2*a(n-9) + a(n-10) (n>9)
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n) {
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cached_len = 0;
    static size_t last_computed = 0;

    /* helper to grow cache */
    if (cached_len == 0) {
        cached_len = 32;
        cache = (seq_t *)calloc(cached_len, sizeof(seq_t));
        /* initial 10 terms */
        cache[0] = 0; cache[1] = 0; cache[2] = 0; cache[3] = 1; cache[4] = 1;
        cache[5] = 1; cache[6] = 1; cache[7] = 2; cache[8] = 3; cache[9] = 4;
        last_computed = 9;
    }

    if ((size_t)n > cached_len - 1) {
        size_t new_cap = cached_len;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        for (size_t i = cached_len; i < new_cap; ++i) cache[i] = 0;
        cached_len = new_cap;
    }

    /* compute missing values up to n */
    for (size_t k = last_computed + 1; k <= (size_t)n; ++k) {
        seq_t val;
        if (k <= 9) {
            /* already initialized */
            val = cache[k];
        } else {
            val = 2 * cache[k-1];
            val -= 2 * cache[k-2];
            val += 2 * cache[k-3];
            val -= cache[k-4];
            val += cache[k-6];
            val -= 2 * cache[k-7];
            val += 2 * cache[k-8];
            val -= 2 * cache[k-9];
            val += cache[k-10];
            cache[k] = val;
        }
    }
    last_computed = (size_t)n;
    return cache[n];
}
