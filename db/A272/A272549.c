/*
 * A272549  Expansion of x*(1 + 5*x - 3*x^2 + 7*x^3 + 3*x^4 + 3 *x^5 - x^6 +
 *          x^7)/((1 - x)^3*(1 + x + x^2 + x^3)^2).
 * https://oeis.org/A272549
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A272549:  a(n) = a(n-1) + 2*a(n-4) - 2*a(n-5) - a(n-8) + a(n-9)
 *  Initial values: a(0)=0, a(1)=1, a(2)=6, a(3)=3, a(4)=10,
 *  a(5)=15, a(6)=28, a(7)=21, a(8)=36.  */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* static cache for memoized values */
    static seq_t *cache = NULL;
    static size_t   cap    = 0;
    static idx_t    last_computed = 0;   /* highest index that has been computed */
    static bool     initialized  = false;

    /* Ensure capacity and fill the first nine terms if needed */
    if (!initialized) {
        /* At least 9 entries are needed, but grow to n+1 if larger */
        size_t need_cap = (n > 8 ? (size_t)n + 1 : 9);
        size_t init_cap = need_cap > 16 ? need_cap : 16;
        cache = (seq_t*)malloc(init_cap * sizeof(seq_t));
        cap = init_cap;

        /* Initial values */
        seq_t init[9] = {0, 1, 6, 3, 10, 15, 28, 21, 36};
        for (idx_t i = 0; i < 9; ++i)
            cache[i] = init[i];

        last_computed = 8;
        initialized  = true;
    } else {
        /* Grow cache if needed */
        if ((size_t)n + 1 > cap) {
            size_t new_cap = cap;
            while ((size_t)n + 1 > new_cap)
                new_cap *= 2;
            cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
            cap   = new_cap;
        }
    }

    /* Compute missing terms up to n using the recurrence */
    if (n > last_computed) {
        for (idx_t i = last_computed + 1; i <= n; ++i) {
            seq_t term = cache[i - 1];
            term += 2 * cache[i - 4];
            term -= 2 * cache[i - 5];
            term -= cache[i - 8];
            term += cache[i - 9];
            cache[i] = term;
        }
        last_computed = n;
    }

    return cache[n];
}
