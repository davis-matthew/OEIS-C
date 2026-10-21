/*
 * A296390  Number of n X 2 0..1 arrays with each 1 horizontally, vertically
 *          or antidiagonally adjacent to 1 or 3 neighboring 1's.
 * https://oeis.org/A296390
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 2*a(n-2) + 2*a(n-3) - a(n-4) + 4*a(n-5) - a(n-6)
 *            - 4*a(n-7) + a(n-9) Initial values: a(1)=2, a(2)=6, a(3)=11,
 *            a(4)=28, a(5)=63, a(6)=139, a(7)=328, a(8)=735, a(9)=1684
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 26, u64 n <= 53, u128 n <= 107,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache: indices 1..cap-1 are valid a-values; index 0 unused */
    static seq_t *cache = NULL;
    static size_t cap = 0;          /* number of slots currently allocated   */
    static idx_t computed = 0;      /* largest n for which a(n) is computed */

    /* Pre‑defined initial terms (index 0 unused) */
    static const seq_t init[10] = {0U,
                                   2U, 6U, 11U, 28U, 63U,
                                   139U, 328U, 735U, 1684U};

    /* Reject indices below the OEIS offset */
    if (n < 1)
        return (seq_t)0;

    /* Allocate enough storage if necessary */
    if ((size_t)n + 1 > cap) {
        size_t newcap = ((size_t)n + 1) * 2;   /* exponential growth */
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp)
            return (seq_t)0;   /* allocation failure */
        cache = tmp;
        /* Initialise new slots */
        for (size_t i = cap; i < newcap; ++i)
            cache[i] = (i < 10) ? init[i] : (seq_t)0;
        cap = newcap;
    }

    /* Base case for small n */
    if (n < 10)
        return cache[(size_t)n];

    /* Ensure cached values are up‑to‑date */
    if (computed < 9)
        computed = 9;        /* the initial block defines up to a(9) */

    for (idx_t i = computed + 1; i <= n; ++i) {
        seq_t v = cache[i-1]
                + 2 * cache[i-2]
                + 2 * cache[i-3]
                - cache[i-4]
                + 4 * cache[i-5]
                - cache[i-6]
                - 4 * cache[i-7]
                + cache[i-9];
        cache[(size_t)i] = v;
    }
    computed = n;
    return cache[(size_t)n];
}
