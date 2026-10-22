/*
 * A369431  a(n) is the number of permutations of [n] which avoid the patterns
 *          1234, 1324, 1342, and 2413.
 * https://oeis.org/A369431
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 20, u64 n <= 39, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = 5*a(n-1) - 7*a(n-2) + 4*a(n-3) for n >= 4
 * Initial values: a(0)=1, a(1)=1, a(2)=2, a(3)=6
 *
 * The sequence satisfies a linear recurrence of order 3, so we
 * compute it iteratively and cache previously computed terms.
 * This implementation works with any seq_t type that supports
 * +, -, *, / with truncating semantics, and with any idx_t type.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Computes the n-th term of OEIS sequence A369431.  */
seq_t a(idx_t n)
{
    /* Static cache for previously computed terms */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;      /* current capacity of cache   */
    static size_t cache_len = 0;      /* number of terms currently kept (max index + 1) */
    static bool initialized = false; /* flag to initialise base cases */

    /* Helper: expand cache capacity if necessary */
    if (n + 1 > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 8;
        while (new_cap <= n + 1) new_cap *= 2;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* Initialise base values on first use */
    if (!initialized) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)2;
        cache[3] = (seq_t)6;
        cache_len = 4;
        initialized = true;
    }

    /* Compute missing terms up to n */
    if (n >= cache_len) {
        for (size_t i = cache_len; i <= (size_t)n; ++i) {
            cache[i] = (seq_t)5 * cache[i-1] - (seq_t)7 * cache[i-2] + (seq_t)4 * cache[i-3];
        }
        cache_len = (size_t)n + 1;
    }

    return cache[(size_t)n];
}
