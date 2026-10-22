/*
 * A396953  Number of elements of word-length n in the reflection group of a
 *          seven-dimensional orthoscheme with distinct edge lengths.
 * https://oeis.org/A396953
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1 + x)^4 / (1 - 4*x + 3*x^2 + 2*x^3 - x^4) Recurrence:
 *            a(n) = 4*a(n-1) - 3*a(n-2) - 2*a(n-3) + a(n-4) (n>=5) Initial
 *            values: a(0)=1, a(1)=8, a(2)=35, a(3)=118, a(4)=353
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 20, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t cache_capacity = 0;
    static const seq_t init_values[5] = { (seq_t)1, (seq_t)8, (seq_t)35, (seq_t)118, (seq_t)353 };

    if (n < 5) return init_values[n];

    /* Allocate cache if needed */
    if (cache == NULL) {
        size_t cap = (size_t)n + 1;
        if (cap < 16) cap = 16;
        cache = (seq_t*)calloc(cap, sizeof(seq_t));
        cache_capacity = cap;

        /* Initialize first five terms */
        cache[0] = init_values[0];
        cache[1] = init_values[1];
        cache[2] = init_values[2];
        cache[3] = init_values[3];
        cache[4] = init_values[4];
        cache_size = 5;
    } else if ((size_t)n + 1 > cache_capacity) {
        size_t cap = cache_capacity;
        while (cap <= (size_t)n) cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, cap * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0; /* allocation failed */
        cache = tmp;
        cache_capacity = cap;
    }

    if ((size_t)n < cache_size) return cache[n];

    /* Compute missing terms using the recurrence */
    for (size_t i = cache_size; i <= (size_t)n; ++i) {
        seq_t t1 = 4 * cache[i - 1] + cache[i - 4];
        seq_t t2 = 3 * cache[i - 2] + 2 * cache[i - 3];
        cache[i] = t1 - t2;
    }
    cache_size = (size_t)n + 1;
    return cache[n];
}
