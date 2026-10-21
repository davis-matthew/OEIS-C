/*
 * A251229  Number of (n+1) X (1+1) 0..3 arrays with every 2 X 2 subblock
 *          summing to 6 and no 2 X 2 subblock having exactly two nonzero
 *          entries.
 * https://oeis.org/A251229
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7*a(n-1) - 12*a(n-2) - 8*a(n-3) + 24*a(n-4) initial
 *            values: a(1)=38, a(2)=110, a(3)=322, a(4)=966
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 17, u64 n <= 36, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* memoisation array (index 0 unused)       */
static size_t alloc_size = 0;    /* number of elements allocated             */
static size_t last_computed = 0; /* greatest n for which cache[n] is set      */

/* Ensure the cache is large enough to hold indices up to 'need' (inclusive). */
static void cache_ensure(size_t need)
{
    if (alloc_size >= need + 1)
        return;
    size_t new_size = alloc_size ? alloc_size << 1 : 8;
    while (new_size < need + 1)
        new_size <<= 1;
    /* explicit cast for C++ compatibility */
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    cache = tmp;
    alloc_size = new_size;
}

/* Initialise the cache with the first four terms of the sequence. */
static void cache_init(void)
{
    if (cache)
        return;                     /* already initialised */
    cache_ensure(4);                 /* indices 0..4 */
    cache[0] = 0;                    /* unused */
    cache[1] = 38;
    cache[2] = 110;
    cache[3] = 322;
    cache[4] = 966;
    last_computed = 4;
}

/* Return a(n) for n >= 1.  Calls may be in any order. */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                    /* behaviour for n<1 is irrelevant */

    cache_init();
    size_t N = (size_t)n;

    cache_ensure(N);                 /* make sure we can store up to a(N) */

    /* Compute any missing terms up to N. */
    if (N > last_computed) {
        for (size_t i = last_computed + 1; i <= N; ++i) {
            seq_t val = 7 * cache[i - 1];
            val -= 12 * cache[i - 2];
            val -= 8 * cache[i - 3];
            val += 24 * cache[i - 4];
            cache[i] = val;
        }
        last_computed = N;
    }

    return cache[N];
}
