/*
 * A267473  Number of length-5 0..n arrays with no following elements larger
 *          than the first repeated value.
 * https://oeis.org/A267473
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^5 + (37/12)n^4 + (11/2)n^3 + (77/12)n^2 + 4n + 1.
 *            Recurrence for n>6: a(n) = 6*a(n-1) - 15*a(n-2) + 20*a(n-3) -
 *            15*a(n-4) + 6*a(n-5) - a(n-6). Initial values: a(1)=21,
 *            a(2)=160, a(3)=712, a(4)=2285, a(5)=5921, a(6)=13216
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 83, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t *cache = NULL;          /* array [0 .. cached_n], index 0 unused */
static size_t cached_n = 0;          /* highest n that has been computed */
static size_t cache_cap = 0;         /* allocated size of cache (>= cached_n+1) */

seq_t a(idx_t n)
{
    if (n == 0) {          /* behaviour for index 0 is irrelevant */
        return (seq_t)0;
    }

    /* Ensure cache has enough space */
    if (cache == NULL || n > cache_cap - 1) {
        size_t new_cap = (n > 64 ? n + 1 : 65); /* grow a bit */
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) {
            /* Out of memory: fail gracefully */
            /* In practice this should not happen for n <= 10000 */
            return (seq_t)0;
        }
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Initialize first 6 terms on first use */
    if (cached_n < 6) {
        /* Initialize up to 6 if not already done, or extend further if needed */
        if (cached_n == 0) {
            cache[1] = (seq_t)21;
            cache[2] = (seq_t)160;
            cache[3] = (seq_t)712;
            cache[4] = (seq_t)2285;
            cache[5] = (seq_t)5921;
            cache[6] = (seq_t)13216;
        }
        cached_n = 6;
    }

    /* Compute further terms if necessary */
    if ((size_t)n > cached_n) {
        for (size_t i = cached_n + 1; i <= (size_t)n; ++i) {
            seq_t val = 6 * cache[i - 1]
                      - 15 * cache[i - 2]
                      + 20 * cache[i - 3]
                      - 15 * cache[i - 4]
                      + 6  * cache[i - 5]
                      -    cache[i - 6];
            cache[i] = val;
        }
        cached_n = (size_t)n;
    }

    return cache[(size_t)n];
}
