/*
 * A356716  a(n) is the integer w such that (c(n)^2, -d(n)^2, -w) is a
 *          primitive solution to the Diophantine equation 2*x^3 + 2*y^3 + z^3
 *          = 11^3, where c(n) = F(n+2) + (-1)^n * F(n-3), d(n) = F(n+1) +
 *          (-1)^n * F(n-4) and F(n) is the n-th Fibonacci number (A000045).
 * https://oeis.org/A356716
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 22, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS sequence A356716
 *
 *  Recurrence (for n > 5):
 *      a(n) = a(n-1) + 7*a(n-2) – 7*a(n-3) – a(n-4) + a(n-5)
 *
 *  Initial values:
 *      a(1) = 5
 *      a(2) = 19
 *      a(3) = 31
 *      a(4) = 101
 *      a(5) = 179
 *
 *  The function keeps a static cache so that calls in arbitrary order
 *  run in O(1) per term once the higher terms have been computed.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache: index 0 left unused to keep 1‑based indices */
    static seq_t *cache      = NULL;
    static idx_t cache_limit = 0;   /* highest n prebuilt */
    static size_t cache_cap   = 0;   /* allocated size (>= cache_limit+1) */

    /* initialise cache with the first five terms */
    if (cache == NULL) {
        cache_cap   = 6;                     /* indices 0..5 */
        cache       = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) { exit(EXIT_FAILURE); }

        cache[1] = (seq_t)5;
        cache[2] = (seq_t)19;
        cache[3] = (seq_t)31;
        cache[4] = (seq_t)101;
        cache[5] = (seq_t)179;
        cache_limit = 5;
    }

    /* return cached value if already available */
    if (n <= cache_limit) return cache[n];

    /* enlarge cache allocation if necessary */
    if ((idx_t)cache_cap <= n) {
        size_t new_cap = cache_cap;
        while ((idx_t)new_cap <= n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) { exit(EXIT_FAILURE); }
        cache     = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms up to n */
    for (idx_t i = cache_limit + 1; i <= n; ++i) {
        /* a(i) = a(i-1) + 7*a(i-2) – 7*a(i-3) – a(i-4) + a(i-5) */
        seq_t term = cache[i-1];
        term += cache[i-2] * (seq_t)7;
        term -= cache[i-3] * (seq_t)7;
        term -= cache[i-4];
        term += cache[i-5];
        cache[i] = term;
    }

    cache_limit = n;
    return cache[n];
}
