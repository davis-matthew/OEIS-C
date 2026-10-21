/*
 * A279262  Number of n X 2 0..1 arrays with no element equal to a strict
 *          majority of its horizontal and vertical neighbors, with the
 *          exception of exactly one element, and with new values introduced
 *          in order 0 sequentially upwards.
 * https://oeis.org/A279262
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) - a(n-2) - 3*a(n-3) + a(n-4) + a(n-5), with
 *            a(1)=0, a(2)=4, a(3)=10, a(4)=20, a(5)=38
 * Range:     u8 n <= 8, u16 n <= 18, u32 n <= 40, u64 n <= 85, u128 n <= 176,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <assert.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;          /* largest index currently cached */

    assert(n >= 1);                       /* offset is 1 */

    idx_t old_size = cache_size;
    if (n > cache_size) {
        /* allocate or grow storage for indices 0..n (index 0 unused) */
        size_t new_bytes = (size_t)(n + 1) * sizeof(seq_t);
        seq_t *new_cache;
        if (cache == NULL) {
            new_cache = (seq_t *)calloc(n + 1, sizeof(seq_t));
            if (new_cache == NULL) abort();
        } else {
            new_cache = (seq_t *)realloc(cache, new_bytes);
            if (new_cache == NULL) abort();
            /* zero the newly added entries */
            memset((void *)(new_cache + old_size + 1),
                   0,
                   (n - old_size) * sizeof(seq_t));
        }
        cache = new_cache;
        cache_size = n;
    }

    /* initialise the first five values (safe to assign each time) */
    if (cache_size >= 1) cache[1] = 0;
    if (cache_size >= 2) cache[2] = 4;
    if (cache_size >= 3) cache[3] = 10;
    if (cache_size >= 4) cache[4] = 20;
    if (cache_size >= 5) cache[5] = 38;

    /* compute any new terms using the recurrence */
    idx_t start = (old_size >= 6) ? old_size + 1 : 6;
    for (idx_t i = start; i <= cache_size; ++i) {
        /* a(i) = 3*a(i-1) - a(i-2) - 3*a(i-3) + a(i-4) + a(i-5) */
        seq_t t1 = cache[i - 1] * 3 + cache[i - 4] + cache[i - 5];
        seq_t t2 = cache[i - 2] + cache[i - 3] * 3;
        cache[i] = t1 - t2;
    }

    return cache[n];
}
