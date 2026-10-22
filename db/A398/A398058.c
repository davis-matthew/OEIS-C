/*
 * A398058  Lower (1,4/3) midsequence of triangular numbers (A000217) and
 *          squares (A000290); see Comments.
 * https://oeis.org/A398058
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) - a(n-2) + a(n-3) - 2*a(n-4) + a(n-5), a(0) = 0,
 *            a(1) = 2, a(2) = 8, a(3) = 18, a(4) = 31
 * Range:     u8 n <= 11, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* static cache for memoisation */
static seq_t *cache = NULL;          /* array of computed values */
static size_t cache_cap = 0;         /* current allocated capacity */
static size_t cache_len = 0;         /* number of values stored */

/* ensure the cache can hold at least 'needed' elements */
static void ensure_capacity(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 16;
    while (new_cap <= needed)
        new_cap <<= 1;
    /* realloc returns void* in both C and C++; cast explicitly */
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();                     /* insufficient memory */
    cache = tmp;
    cache_cap = new_cap;
}

/* compute a(n) using the recurrence and memoisation */
seq_t a(idx_t n)
{
    /* Return cached value if already computed */
    if ((size_t)n < cache_len)
        return cache[(size_t)n];

    /* Ensure the cache can accommodate all indices up to n */
    size_t needed = (size_t)n + 1;
    ensure_capacity(needed);

    /* Fill cache from the last computed position up to n */
    for (size_t i = cache_len; i <= n; ++i) {
        seq_t val;
        switch ((idx_t)i) {          /* initial values for n = 0..4 */
            case 0:  val = 0;        break;
            case 1:  val = 2;        break;
            case 2:  val = 8;        break;
            case 3:  val = 18;       break;
            case 4:  val = 31;       break;
            default:
                /* recurrence: a(i) = 2*a(i-1) - a(i-2) + a(i-3) - 2*a(i-4) + a(i-5) */
                val = 2 * cache[i-1] - cache[i-2] + cache[i-3] - 2 * cache[i-4] + cache[i-5];
        }
        cache[i] = val;
    }

    cache_len = (size_t)n + 1;
    return cache[(size_t)n];
}
