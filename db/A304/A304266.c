/*
 * A304266  Number of nX4 0..1 arrays with every element unequal to 0, 1, 5 or
 *          8 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A304266
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1)+a(n-2)+5*a(n-3)+a(n-4)-3*a(n-5)-3*a(n-6) (n>7)
 *            Initial values: a(1)=5, a(2)=5, a(3)=10, a(4)=21, a(5)=50,
 *            a(6)=116, a(7)=259
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 26, u64 n <= 53, u128 n <= 106,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#if defined(__cplusplus)
#include <new>
#endif

/* Static storage for memoisation */
static seq_t *cache = NULL;
static idx_t   cache_size = 0;   /* number of allocated elements (>=0) */
static idx_t   cached_max = 0;   /* largest index computed so far */

/* Allocate or grow the cache to hold at least (n+1) entries */
static void
ensure_cache_size(idx_t n)
{
    idx_t needed = n + 1;
    if (cache_size >= needed)
        return;

    seq_t *tmp;
#if defined(__cplusplus)
    tmp = reinterpret_cast<seq_t*>(::operator new(needed * sizeof(seq_t)));
#else
    tmp = (seq_t*)malloc(needed * sizeof(seq_t));
#endif
    if (!tmp)
        return; /* allocation failed – caller will handle zero value */

    /* copy old values if any */
    if (cache) {
#if defined(__cplusplus)
        for (idx_t i = 0; i < cache_size; ++i)
            tmp[i] = cache[i];
        ::operator delete(cache);
#else
        memcpy(tmp, cache, cache_size * sizeof(seq_t));
        free(cache);
#endif
    }

    cache      = tmp;
    cache_size = needed;
}

/* Initialise the base values a(1)..a(7) if not already done */
static void
init_base_values(void)
{
    if (cached_max >= 7)
        return;

    /* ensure indices up to 7 exist */
    ensure_cache_size(7);

    cache[1] = (seq_t)5;
    cache[2] = (seq_t)5;
    cache[3] = (seq_t)10;
    cache[4] = (seq_t)21;
    cache[5] = (seq_t)50;
    cache[6] = (seq_t)116;
    cache[7] = (seq_t)259;

    /* mark as computed up to 7 */
    if (cached_max < 7)
        cached_max = 7;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;          /* offset is 1 – values below are undefined */

    /* make sure cache is big enough for n */
    ensure_cache_size(n);

    /* initialise base values once */
    init_base_values();

    /* compute values from cached_max+1 up to n */
    for (idx_t i = cached_max + 1; i <= n; ++i) {
        /* recurrence for i>7, base already filled for i<=7 */
        seq_t term = cache[i-1] + cache[i-2] + (seq_t)5 * cache[i-3]
                     + cache[i-4] - (seq_t)3 * cache[i-5]
                     - (seq_t)3 * cache[i-6];
        cache[i] = term;
    }
    if (n > cached_max)
        cached_max = n;

    return cache[n];
}
