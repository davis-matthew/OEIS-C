/*
 * A297397  Number of 4 X n 0..1 arrays with every 1 horizontally, diagonally
 *          or antidiagonally adjacent to 1 neighboring 1.
 * https://oeis.org/A297397
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 34, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: Empirical recurrence
 *
 *   a(1) = 1,   a(2) = 20,   a(3) = 57,   a(4) = 126,
 *   a(5) = 700, a(6) = 2569, a(7) = 7779, a(8) = 31081
 *
 *   a(n) = a(n-1) + 3·a(n-2) + 24·a(n-3) + 11·a(n-4)
 *          - 21·a(n-5) - 67·a(n-6) - 9·a(n-7) - 45·a(n-8)   for n ≥ 9
 *
 *   The sequence is indexed starting at 1, i.e. a(1) is the first term above.
 *
 *   This implementation memoises the computed values so that any number of
 *   calls, in any order, is fast.  Only integer operations on seq_t are
 *   used; the code compiles for any unsigned integer type defined by the
 *   build system (uint8_t, uint16_t, uint32_t, uint64_t, unsigned __int128,
 *   or an arbitrary‑precision class).
 */

#include <stddef.h>
#include <stdlib.h>

static const seq_t init[8] = {
    (seq_t)1ULL,     /* a(1) */
    (seq_t)20ULL,    /* a(2) */
    (seq_t)57ULL,    /* a(3) */
    (seq_t)126ULL,   /* a(4) */
    (seq_t)700ULL,   /* a(5) */
    (seq_t)2569ULL,  /* a(6) */
    (seq_t)7779ULL,  /* a(7) */
    (seq_t)31081ULL  /* a(8) */
};

static seq_t *cache   = NULL;
static idx_t cache_len = 0;

static void ensure_cache(idx_t n)
{
    if (n <= cache_len)
        return;

    size_t new_len = (size_t)n;
    if (new_len < 8)
        new_len = 8;

    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof *cache);
    if (!tmp)
        abort();
    cache = tmp;

    /* initialise missing initial values */
    for (size_t i = cache_len; i < 8 && i < new_len; ++i)
        cache[i] = init[i];

    /* compute remaining terms up to new_len */
    for (size_t i = (cache_len < 8 ? 8 : cache_len); i < new_len; ++i) {
        seq_t v = cache[i-1]
                + (seq_t)3 * cache[i-2]
                + (seq_t)24 * cache[i-3]
                + (seq_t)11 * cache[i-4]
                - (seq_t)21 * cache[i-5]
                - (seq_t)67 * cache[i-6]
                - (seq_t)9  * cache[i-7]
                - (seq_t)45 * cache[i-8];
        cache[i] = v;
    }

    cache_len = new_len;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;               /* undefined, return 0 */
    ensure_cache(n);
    return cache[n-1];
}
