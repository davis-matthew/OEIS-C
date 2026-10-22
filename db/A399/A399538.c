/*
 * A399538  G.f. A(x) satisfies: A(x) = x * (1 + A(2*x)) / (1 - x^3).
 * https://oeis.org/A399538
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n for n < 3; a(n) = 2^(n-1) * a(n-1) + a(n-3)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 8, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Dynamic cache used for memoisation. */
static seq_t *cache   = NULL;   /* array of computed values               */
static size_t cap     = 0;      /* current capacity of the array           */
static idx_t  last    = 0;      /* largest index that has been filled      */
static seq_t pow_of_two = 0;    /* holds 2^(i-1) for the next i to compute */

/* Ensure the cache can accommodate indices up to n. */
static void grow_cache(idx_t n)
{
    if ((size_t)(n + 1) > cap) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cap   = new_cap;
    }
}

/* Main function required by the framework. */
seq_t a(idx_t n)
{
    /* Initialisation on first call. */
    if (cache == NULL) {
        grow_cache(2);          /* capacity for indices 0..2 */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)2;
        last = 2;
        /* pow_of_two must be 2^(last) = 2^2 = 4 for the next i=3 */
        pow_of_two = (seq_t)1 << last;   /* safe: last == 2 */
    }

    /* If the requested term is already known, return it. */
    if (n <= last) {
        return cache[n];
    }

    /* Extend the cache to the required size. */
    grow_cache(n);

    /* Compute missing terms iteratively. */
    for (idx_t i = last + 1; i <= n; ++i) {
        /* cache[i] = pow_of_two * cache[i-1] + cache[i-3]; */
        seq_t prod = pow_of_two * cache[i - 1];
        cache[i] = prod + cache[i - 3];
        /* Prepare pow_of_two for the next iteration: 2^(i) = 2^(i-1) * 2 */
        pow_of_two *= (seq_t)2;
    }

    last = n;
    return cache[n];
}
