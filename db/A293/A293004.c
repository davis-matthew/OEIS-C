/*
 * A293004  Expansion of 2*x^2 / (x^3 + x^2 - 3x + 1).
 * https://oeis.org/A293004
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) - a(n-2) - a(n-3) for n > 2, a(0) = 0, a(1) = 0,
 *            a(2) = 2
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 25, u64 n <= 51, u128 n <= 101,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *a_cache = NULL;
static idx_t a_cache_size = 0;    /* number of slots allocated */
static idx_t a_cache_max = (idx_t)-1; /* highest n computed, -1 means none */

/* Return a(n) for OEIS A293004 */
seq_t a(idx_t n)
{
    /* First call: allocate and compute up to n */
    if (a_cache == NULL) {
        a_cache_size = (n + 1 > 32) ? (n + 1) : 32;
        a_cache = (seq_t *)malloc(a_cache_size * sizeof(seq_t));
        if (a_cache == NULL) abort();   /* allocation failure */
        a_cache[0] = 0;
        a_cache[1] = 0;
        if (n >= 2) a_cache[2] = 2;
        for (idx_t i = 3; i <= n; ++i) {
            a_cache[i] = 3 * a_cache[i-1] - a_cache[i-2] - a_cache[i-3];
        }
        a_cache_max = n;
        return a_cache[n];
    }

    /* n is already computed */
    if (n <= a_cache_max) {
        return a_cache[n];
    }

    /* Grow cache to accommodate n */
    if ((idx_t)(a_cache_size) <= n) {
        size_t new_size = (n + 1 > a_cache_size * 2) ? (n + 1) : a_cache_size * 2;
        seq_t *new_ptr = (seq_t *)realloc(a_cache, new_size * sizeof(seq_t));
        if (new_ptr == NULL) abort();
        a_cache = new_ptr;
        a_cache_size = new_size;
    }

    /* Compute missing values up to n */
    for (idx_t i = a_cache_max + 1; i <= n; ++i) {
        if (i == 0)           a_cache[i] = 0;
        else if (i == 1)      a_cache[i] = 0;
        else if (i == 2)      a_cache[i] = 2;
        else                  a_cache[i] = 3 * a_cache[i-1] - a_cache[i-2] - a_cache[i-3];
    }
    a_cache_max = n;
    return a_cache[n];
}
