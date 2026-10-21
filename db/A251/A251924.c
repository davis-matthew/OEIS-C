/*
 * A251924  Numbers n such that the sum of the triangular numbers T(n) and
 *          T(n+1) is equal to a hexagonal number H(m) for some m.
 * https://oeis.org/A251924
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A251924:
   a(n) = the n-th n such that T(n)+T(n+1) is a hexagonal number.
   Recurrence: a(1)=0, a(2)=34, a(3)=1188, a(n)=35*a(n-1)-35*a(n-2)+a(n-3) for n≥4 */
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;          /* array indexed from 1 */
    static size_t cache_cap = 0;         /* allocated capacity */
    static idx_t cached_n = 0;           /* highest n cached */
    static bool initialized = false;     /* initial terms set? */

    /* Ensure array has capacity for indices up to 'n'. */
    if (cache_cap <= n) {
        size_t new_cap = cache_cap ? cache_cap : 4;
        while (new_cap <= n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, (new_cap + 1) * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Initialize the first three values once. */
    if (!initialized) {
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)34;
        cache[3] = (seq_t)1188;
        cached_n = 3;
        initialized = true;
    }

    /* If the requested value is already cached, return it. */
    if (n <= cached_n)
        return cache[n];

    /* Compute values up to n using the recurrence. */
    for (idx_t i = cached_n + 1; i <= n; ++i) {
        cache[i] = (seq_t)35 * cache[i - 1] - (seq_t)35 * cache[i - 2] + cache[i - 3];
    }
    cached_n = n;
    return cache[n];
}
