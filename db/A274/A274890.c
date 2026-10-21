/*
 * A274890  Number of n X 3 0..2 arrays with no element equal to any value at
 *          offset (0,-1) (-1,-1) or (-2,0) and new values introduced in order
 *          0..2.
 * https://oeis.org/A274890
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 4*a(n-2) - 6*a(n-4) - a(n-5) + 4*a(n-6) - a(n-8)
 *            for n > 10, with initial values a(1)=2, a(2)=12, a(3)=16,
 *            a(4)=38, a(5)=84, a(6)=192, a(7)=436, a(8)=990, a(9)=2253,
 *            a(10)=5121
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 26, u64 n <= 53, u128 n <= 107,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Include the standard memory allocation header. */
#include <stdlib.h>

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Cache of previously computed values. */
    static seq_t *cache   = NULL;
    static size_t cap     = 0;   /* allocated size of cache (in seq_t elements) */
    static idx_t  max_n   = 0;   /* highest index for which cache[n] is valid */

    /* Ensure cache is initialized. */
    if (cache == NULL) {
        cap = 16;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        /* In the unlikely event malloc fails, return 0. */
        if (cache == NULL) return (seq_t)0;
    }

    /* Grow cache if necessary. */
    if ((size_t)(n + 1) > cap) {
        size_t newcap = cap;
        while ((size_t)(n + 1) > newcap)
            newcap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;
        cache = tmp;
        cap = newcap;
    }

    /* Return cached value if already known. */
    if (n <= max_n) return cache[n];

    /* Initial values for n = 1 .. 10. */
    static const seq_t init[11] = {
        0,      /* unused: a(0) */
        2, 12, 16, 38, 84,
        192, 436, 990, 2253, 5121
    };

    /* Compute values up to n. */
    for (idx_t i = max_n + 1; i <= n; ++i) {
        if (i <= 10) {
            cache[i] = init[i];
        } else {
            /* Apply the empirical recurrence. */
            seq_t val = cache[i - 1]
                      + 4 * cache[i - 2]
                      - 6 * cache[i - 4]
                      - cache[i - 5]
                      + 4 * cache[i - 6]
                      - cache[i - 8];
            cache[i] = val;
        }
    }

    max_n = n;
    return cache[n];
}
