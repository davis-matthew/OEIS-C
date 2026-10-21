/*
 * A281766  Number of 2 X n 0..1 arrays with no element unequal to a strict
 *          majority of its horizontal, diagonal and antidiagonal neighbors,
 *          with the exception of exactly one element, and with new values
 *          introduced in order 0 sequentially upwards.
 * https://oeis.org/A281766
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) computed by the order‑9 linear recurrence a(n) = 4*a(n-1) +
 *            a(n-2) - 16*a(n-3) - a(n-4) + 30*a(n-5) + 4*a(n-6) - 24*a(n-7) -
 *            4*a(n-8) + 8*a(n-9) Initial values: a(1)=0, a(2)=4, a(3)=14,
 *            a(4)=40, a(5)=110, a(6)=280, a(7)=698, a(8)=1696, a(9)=4052 The
 *            function memoises results so that calls in...
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 25, u64 n <= 54, u128 n <= 110,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* NULL, size_t */
#include <stdlib.h>   /* malloc, realloc, free */

seq_t a(idx_t n)
{
    /* Static cache for memoised values.  The cache[0] slot is unused
       to keep 1‑based indexing simple. */
    static seq_t *cache   = NULL;
    static idx_t cache_sz = 0;   /* allocated size (elements) */
    static idx_t cached_max = 0; /* largest n computed so far */

    /* Initial values for n = 1..9 to seed the recurrence. */
    static const seq_t init[9] = {
        0, 4, 14, 40, 110, 280, 698, 1696, 4052
    };

    if (n < 1) {
        return (seq_t)0;   /* offset not defined */
    }

    /* Expand cache if necessary.  Use (seq_t*) cast to satisfy C++ mode. */
    if (cache_sz <= n) {
        seq_t *tmp = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (tmp == NULL) {
            /* Memory allocation failed; return 0 as a safe fallback. */
            return (seq_t)0;
        }
        cache   = tmp;
        cache_sz = n + 1;
        /* Initialise unused slot 0 to 0 for convenience. */
        if (cache_sz > 0) cache[0] = (seq_t)0;
    }

    /* Compute missing terms up to n using the recurrence. */
    if (n > cached_max) {
        idx_t i;
        for (i = cached_max + 1; i <= n; ++i) {
            if (i <= 9) {
                cache[i] = init[i - 1];
            } else {
                seq_t t1 = cache[i - 1];
                seq_t t2 = cache[i - 2];
                seq_t t3 = cache[i - 3];
                seq_t t4 = cache[i - 4];
                seq_t t5 = cache[i - 5];
                seq_t t6 = cache[i - 6];
                seq_t t7 = cache[i - 7];
                seq_t t8 = cache[i - 8];
                seq_t t9 = cache[i - 9];
                cache[i] = (seq_t)(
                    4 * t1 + t2 - 16 * t3 - t4 + 30 * t5
                    + 4 * t6 - 24 * t7 - 4 * t8 + 8 * t9
                );
            }
        }
        cached_max = n;
    }

    return cache[n];
}
