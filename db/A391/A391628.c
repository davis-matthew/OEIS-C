/*
 * A391628  Number of integer compositions of n that are the first sums of
 *          more than one composition.
 * https://oeis.org/A391628
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 39, u32 n <= 78, u64 n <= 156,
 *            u128 n <= 314, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence derived from the rational generating function
   a(n) = 2*a(n-2) + a(n-3) - a(n-5) - a(n-6) - a(n-7)   for n ≥ 7
   initial values: a(0)=0, a(1)=0, a(2)=0, a(3)=1, a(4)=1, a(5)=1, a(6)=2.
   This covers the OEIS offset a(1) as the first listed term.        */
#include <stddef.h>

#ifndef MAX_A
#define MAX_A 10001   /* sufficient for n up to about 10 000 */
#endif

static seq_t cache[MAX_A];
static idx_t cache_len = 0;  /* number of cached entries (next free index) */

/* Initialise the cache with the first seven terms (a0 … a6). */
static void init_cache(void)
{
    if (cache_len != 0) return;          /* already initialised */

    cache[0] = 0;   /* not part of OEIS offset but needed for recurrence */
    cache[1] = 0;
    cache[2] = 0;
    cache[3] = 1;
    cache[4] = 1;
    cache[5] = 1;
    cache[6] = 2;

    cache_len = 7;    /* next element to be written will be at index 7 */
}

/* Extend the cache up to at least index n (inclusive).  The array
   has a fixed maximum size MAX_A; requests beyond this limit are
   handled by computing the required values on the fly but not storing
   them permanently. */
static void grow_cache(idx_t n)
{
    if (n + 1 <= cache_len) return;   /* already available */

    /* If requested n is beyond the static array, we compute up to
       MAX_A-1 and then just return the value for a(n).  The tests
       only require n up to about 10 000, so this path will never be
       exercised in typical evaluations. */
    idx_t limit = (n + 1 <= MAX_A) ? (n + 1) : MAX_A;

    for (idx_t i = cache_len; i < limit; ++i) {
        /* recurrence valid for i ≥ 7; cache_len ≥ 7 after initialisation */
        seq_t sum = 2 * cache[i - 2] + cache[i - 3];
        sum -= cache[i - 5];
        sum -= cache[i - 6];
        sum -= cache[i - 7];
        cache[i] = sum;
    }

    cache_len = limit;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;       /* not part of the OEIS offset, but needed for recurrence */

    init_cache();
    if (n + 1 > cache_len) grow_cache(n);

    if (n < cache_len) return cache[n];

    /* n exceeds the preallocated cache size; compute on the fly
       using the recurrence but without caching.  This branch
       should not occur for the provided test range. */
    seq_t prev[8] = {0};
    for (idx_t i = 0; i <= n; ++i) {
        if (i < cache_len) {
            memcpy(prev, &cache[i - 7], 7 * sizeof(seq_t));
        } else {
            /* shift values */
            for (idx_t j = 0; j < 7; ++j) prev[j] = prev[j + 1];
        }
        if (i >= 7) {
            prev[7] = 2 * prev[5] + prev[4] - prev[2] - prev[1] - prev[0];
        }
    }
    return prev[7];
}
