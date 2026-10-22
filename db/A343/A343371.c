/*
 * A343371  a(n) = 1 + Sum_{d|n, d < n} a(d - 1).
 * https://oeis.org/A343371
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 629, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(n)=1+∑_{d|n,d<n} a(d-1) */

/* This implementation uses memoisation.  It works for any
 * seq_t (including an arbitrary‑precision class) and for any
 * idx_t (e.g. uint64_t).  Dynamic allocation of the cache is
 * performed with malloc/realloc; casts are added so the code
 * compiles as both C11 and C++17. */

#include <stdlib.h>
#include <stdint.h>

/* External linkage required: a(idx_t n) */
static seq_t *cache = NULL;
static size_t cache_cap = 0;

/* Ensure the cache array holds at least n+1 elements. */
static void ensure_cache(idx_t n)
{
    if (n < cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 1;
    while (new_cap <= (size_t)n)
        new_cap <<= 1;
    /* cast needed for C++ */
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) {
        /* allocation failed – terminate the program */
        exit(EXIT_FAILURE);
    }
    /* initialise the newly allocated part to zero */
    for (size_t i = cache_cap; i < new_cap; ++i)
        tmp[i] = 0;
    cache = tmp;
    cache_cap = new_cap;
}

/* Compute a(n) with memoisation and divisor enumeration. */
seq_t a(idx_t n)
{
    ensure_cache(n);
    if (cache[n] != 0)
        return cache[n];

    seq_t sum = 1;                /* leading constant from the recurrence */

    if (n > 1) {
        for (idx_t d = 1; d <= n / d; ++d) {
            if (n % d == 0) {
                idx_t d1 = d;
                idx_t d2 = n / d;

                if (d1 < n)          /* d1 < n always true when d1< n */
                    sum += a(d1 - 1);

                if (d2 != d1 && d2 < n)
                    sum += a(d2 - 1);
            }
        }
    }

    cache[n] = sum;
    return sum;
}
