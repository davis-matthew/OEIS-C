/*
 * A330642  a(n) is the number of partitions of n with Durfee square of size
 *          <= 4.
 * https://oeis.org/A330642
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies the linear recurrence * a(n) = 2*a(n-1) + a(n-2)
 *            - 2*a(n-3) - a(n-4) - 4*a(n-5) * + 4*a(n-6) + 4*a(n-7) +
 *            2*a(n-8) -10*a(n-10) * + 2*a(n-12)+ 4*a(n-13)+ 4*a(n-14) -
 *            4*a(n-15) * - a(n-16)- 2*a(n-17) + a(n-18)+ 2*a(n-19)- a(n-20) *
 *            for n > 20, with initial values * a(0)=1, a(1)=1, a(...
 * Range:     u8 n <= 16, u16 n <= 43, u32 n <= 204, u64 n <= 4743,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

/* initial values for n = 0 .. 20 */
static const seq_t init[21] = {
    1, 1, 2, 3, 5, 7, 11, 15, 22, 30,
    42, 56, 77, 101, 135, 176, 231, 297, 385, 490, 627
};

/* cache of computed values */
static seq_t *cache = NULL;
static size_t cache_len = 0;

/* compute a(n) */
seq_t a(idx_t n)
{
    /* for n <= 20 simply return the initial value */
    if (n < 21)
        return init[n];

    /* if cache is uninitialised or too short, grow/initialize it */
    if (cache == NULL) {
        /* first time we need values >= 21 */
        cache_len = (size_t)n + 1;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (!cache) abort();          /* out of memory */
        memcpy(cache, init, 21 * sizeof(seq_t));

        /* compute values from 21 up to n */
        for (size_t i = 21; i <= (size_t)n; ++i) {
            seq_t val = 0;
            val += cache[i-1] * 2;
            val += cache[i-2];
            val += cache[i-6] * 4;
            val += cache[i-7] * 4;
            val += cache[i-8] * 2;
            val += cache[i-12] * 2;
            val += cache[i-13] * 4;
            val += cache[i-14] * 4;
            val += cache[i-18];
            val += cache[i-19] * 2;
            val -= cache[i-3] * 2;
            val -= cache[i-4];
            val -= cache[i-5] * 4;
            val -= cache[i-10] * 10;
            val -= cache[i-15] * 4;
            val -= cache[i-16];
            val -= cache[i-17] * 2;
            val -= cache[i-20];
            cache[i] = val;
        }
    } else if ((size_t)n >= cache_len) {
        /* need to extend cache */
        size_t old_len = cache_len;
        cache_len = (size_t)n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, cache_len * sizeof(seq_t));
        if (!new_cache) abort();          /* out of memory */
        cache = new_cache;

        /* compute new entries from old_len to n */
        for (size_t i = old_len; i <= (size_t)n; ++i) {
            seq_t val = 0;
            val += cache[i-1] * 2;
            val += cache[i-2];
            val += cache[i-6] * 4;
            val += cache[i-7] * 4;
            val += cache[i-8] * 2;
            val += cache[i-12] * 2;
            val += cache[i-13] * 4;
            val += cache[i-14] * 4;
            val += cache[i-18];
            val += cache[i-19] * 2;
            val -= cache[i-3] * 2;
            val -= cache[i-4];
            val -= cache[i-5] * 4;
            val -= cache[i-10] * 10;
            val -= cache[i-15] * 4;
            val -= cache[i-16];
            val -= cache[i-17] * 2;
            val -= cache[i-20];
            cache[i] = val;
        }
    }

    return cache[(size_t)n];
}
