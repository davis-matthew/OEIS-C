/*
 * A369846  Number of compositions of 5*n-1 into parts 3 and 5.
 * https://oeis.org/A369846
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A052920(5*n-1) * Recurrence: a(1)=0, a(2)=1, a(3)=4,
 *            a(4)=10, a(5)=21; *
 *            a(n)=5*a(n-1)-10*a(n-2)+11*a(n-3)-5*a(n-4)+a(n-5) for n>5
 * Range:     u8 n <= 8, u16 n <= 14, u32 n <= 26, u64 n <= 51, u128 n <= 101,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* static cache for computed values (1‑based indexing) */
static seq_t *cache = NULL;          /* array of terms */
static idx_t cache_cap = 0;          /* allocated capacity (>=5) */
static idx_t cache_size = 0;         /* largest n with a(n) computed */

/* Ensure the cache contains values up to at least n (n >= 1). */
static void ensure_capacity(idx_t n)
{
    if (n < 1) return;

    /* initial allocation */
    if (cache == NULL) {
        idx_t init = n > 16 ? n : 16;
        cache = (seq_t *)calloc((size_t)(init + 1), sizeof(seq_t));
        cache_cap = init;
    }
    /* grow if needed */
    if (n > cache_cap) {
        idx_t new_cap = cache_cap;
        while (new_cap < n) new_cap <<= 1;
        cache = (seq_t *)realloc(cache, (size_t)(new_cap + 1) * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* set base values (1 through 5) if not already done */
    if (cache_size < 5) {
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)4;
        cache[4] = (seq_t)10;
        cache[5] = (seq_t)21;
        cache_size = 5;
    }

    /* compute missing terms up to n */
    if (n > cache_size) {
        for (idx_t k = cache_size + 1; k <= n; ++k) {
            cache[k] = 5 * cache[k - 1]
                     - 10 * cache[k - 2]
                     + 11 * cache[k - 3]
                     - 5  * cache[k - 4]
                     +     cache[k - 5];
        }
        cache_size = n;
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    ensure_capacity(n);
    return cache[n];
}
