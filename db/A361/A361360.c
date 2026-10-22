/*
 * A361360  Number of nonequivalent noncrossing caterpillars with n edges up
 *          to rotation and reflection.
 * https://oeis.org/A361360
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 17, u64 n <= 32, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 5*a(n-1) + 4*a(n-2) - 34*a(n-3) + 7*a(n-4) + 63*a(n-5)
 *              - 30*a(n-6) - 46*a(n-7) + 31*a(n-8) + 13*a(n-9)
 *              - 14*a(n-10) + 3*a(n-12) - a(n-13),
 *  with initial values
 *  a(0)=1, a(1)=1, a(2)=1, a(3)=3, a(4)=7,
 *  a(5)=28, a(6)=104, a(7)=448, a(8)=1886,
 *  a(9)=8212, a(10)=35556, a(11)=155124, a(12)=675897.
 *
 *  This implementation memoizes the sequence up to the maximum index
 *  seen.  It works for all supported seq_t types: unsigned integer
 *  types (up to 128 bits) and arbitrary-precision integer classes
 *  compiled with C++17.  No external utilities are required.
 */

#include <stdlib.h>

/*Maximum initial capacity (covers the first 13 terms).*/
#define INIT_CAP 16

/* Static cache for computed values. */
static seq_t *cache = NULL;
static idx_t cache_cap = 0;   /* current array capacity */
static idx_t cache_len = 0;   /* number of values stored */

/* Helper to grow the cache to at least new_cap elements. */
static void grow_cache(idx_t new_cap)
{
    if (new_cap <= cache_cap)
        return;
    idx_t new_capacity = (cache_cap > 0) ? cache_cap : INIT_CAP;
    while (new_capacity < new_cap)
        new_capacity *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_capacity * sizeof(seq_t));
    if (!tmp) abort(); /* allocation failed */
    cache = tmp;
    cache_cap = new_capacity;
}

/* Compute a(n). */
seq_t a(idx_t n)
{
    /* Initialise cache with first 13 terms if not done yet. */
    if (!cache)
    {
        cache = (seq_t *)malloc(INIT_CAP * sizeof(seq_t));
        if (!cache) abort();
        cache_cap = INIT_CAP;
        /* 13 known initial terms. */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)3;
        cache[4] = (seq_t)7;
        cache[5] = (seq_t)28;
        cache[6] = (seq_t)104;
        cache[7] = (seq_t)448;
        cache[8] = (seq_t)1886;
        cache[9] = (seq_t)8212;
        cache[10] = (seq_t)35556;
        cache[11] = (seq_t)155124;
        cache[12] = (seq_t)675897;
        cache_len = 13;
    }

    if (n < cache_len)
        return cache[n];

    /* Compute all terms up to n. */
    for (idx_t k = cache_len; k <= n; ++k)
    {
        /* Ensure space for new term. */
        grow_cache(k + 1);

        /* Apply the recurrence.  All operations are
           performed using seq_t arithmetic; constants are
           widened to seq_t before multiplication. */
        seq_t val = (seq_t)5 * cache[k - 1];
        val += (seq_t)4 * cache[k - 2];
        val -= (seq_t)34 * cache[k - 3];
        val += (seq_t)7 * cache[k - 4];
        val += (seq_t)63 * cache[k - 5];
        val -= (seq_t)30 * cache[k - 6];
        val -= (seq_t)46 * cache[k - 7];
        val += (seq_t)31 * cache[k - 8];
        val += (seq_t)13 * cache[k - 9];
        val -= (seq_t)14 * cache[k - 10];
        /* term k-11 has coefficient 0 – omitted. */
        val += (seq_t)3 * cache[k - 12];
        val -= cache[k - 13];

        cache[k] = val;
    }

    cache_len = n + 1;
    return cache[n];
}
