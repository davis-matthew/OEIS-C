/*
 * A348543  Number of partitions of n into 3 parts with at least 1 odd part.
 * https://oeis.org/A348543
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 56, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A348543: Number of partitions of n into 3 parts with at least 1 odd part.
    Recurrence (valid for n ≥ 13):  a(n) = a(n-2)+a(n-4)-a(n-8)-a(n-10)+a(n-12)
    Initial values (n = 1..12):
        a(1) = 0, a(2) = 0, a(3) = 1, a(4) = 1, a(5) = 2,
        a(6) = 2, a(7) = 4, a(8) = 4, a(9) = 7, a(10) = 6,
        a(11) = 10, a(12) = 9
    For n < 1 the function returns 0.
*/

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

/* static cache for computed values */
static seq_t *cache = NULL;
static size_t cache_size = 0;

/* initialise cache with base cases up to index 12 */
static void init_cache(void)
{
    if (cache != NULL)
        return;

    /* allocate enough space for indices 0..12 (13 entries) */
    size_t init_size = 13;
#ifdef __cplusplus
    cache = new seq_t[init_size];
#else
    cache = (seq_t *)malloc(init_size * sizeof(seq_t));
#endif
    if (!cache)
        return; /* allocation failed – leave cache NULL; caller will try again */

    /* zero out (only index 0 is used for the recurrence) */
    for (size_t i = 0; i < init_size; ++i)
        cache[i] = (seq_t)0;

    /* base values */
    cache[1]  = (seq_t)0;
    cache[2]  = (seq_t)0;
    cache[3]  = (seq_t)1;
    cache[4]  = (seq_t)1;
    cache[5]  = (seq_t)2;
    cache[6]  = (seq_t)2;
    cache[7]  = (seq_t)4;
    cache[8]  = (seq_t)4;
    cache[9]  = (seq_t)7;
    cache[10] = (seq_t)6;
    cache[11] = (seq_t)10;
    cache[12] = (seq_t)9;

    cache_size = init_size;
}

/* grow the cache so that it contains values up to n (inclusive) */
static void ensure_cache(idx_t n)
{
    if (n + 1 <= cache_size)
        return;                /* already computed */

    size_t new_size = (size_t)n + 1;
    if (new_size < cache_size + 1)
        new_size = cache_size + 1;

    /* reallocate */
#ifdef __cplusplus
    seq_t *tmp = new seq_t[new_size];
    for (size_t i = 0; i < cache_size; ++i)
        tmp[i] = cache[i];
    delete [] cache;
    cache = tmp;
#else
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) {
        /* allocation failed; keep old cache; caller will fail to compute */
        return;
    }
    cache = tmp;
#endif
    /* zero out new entries */
    for (size_t i = cache_size; i < new_size; ++i)
        cache[i] = (seq_t)0;

    /* compute values from the old size up to new_size-1 */
    for (size_t i = cache_size; i < new_size; ++i) {
        if (i <= 12) {
            /* base values already set */
            continue;
        }
        /* recurrence: a(i) = a(i-2)+a(i-4)-a(i-8)-a(i-10)+a(i-12) */
        seq_t v2  = cache[i-2];
        seq_t v4  = cache[i-4];
        seq_t v8  = cache[i-8];
        seq_t v10 = cache[i-10];
        seq_t v12 = cache[i-12];
        seq_t sum = v2 + v4 + v12;
        seq_t sub = v8 + v10;
        cache[i] = sum - sub;
    }

    cache_size = new_size;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;     /* Undefined for n < 1, return 0 */

    init_cache();
    if (cache == NULL)
        return (seq_t)0;     /* Allocation failed */

    ensure_cache(n);
    if (n + 1 > cache_size)
        return (seq_t)0;     /* Still insufficient (allocation failed) */

    return cache[(size_t)n];
}
