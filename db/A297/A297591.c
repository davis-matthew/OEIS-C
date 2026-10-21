/*
 * A297591  Number of n X 4 0..1 arrays with every 1 horizontally, diagonally
 *          or antidiagonally adjacent to 1 or 5 neighboring 1s.
 * https://oeis.org/A297591
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 6*a(n-2) + 10*a(n-3) + 3*a(n-4) - 13*a(n-5) +
 *            3*a(n-6) - 3*a(n-7) - 4*a(n-8) - a(n-9). Initial values: a(0)=1,
 *            a(1)=4, a(2)=13, a(3)=49, a(4)=154, a(5)=577, a(6)=1977,
 *            a(7)=6962, a(8)=24441, a(9)=85803
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 35, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Precomputed base values for n = 0 .. 9 */
    static const seq_t base[10] = {
        1, 4, 13, 49, 154, 577, 1977, 6962, 24441, 85803
    };

    static seq_t *cache = NULL;    /* dynamic array of computed terms */
    static size_t  cap   = 0;       /* capacity of 'cache' */
    static idx_t   max_i = 0;       /* largest n computed so far */

    /* First call – allocate and load base terms */
    if (cache == NULL) {
        cap = 10;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return 0;  /* allocation failure → undefined */
        for (idx_t i = 0; i < 10; ++i)
            cache[i] = base[i];
        max_i = 9;
    }

    /* Return cached value if already computed */
    if (n <= max_i)
        return cache[n];

    /* Extend cache if necessary */
    if ((size_t)n + 1 > cap) {
        cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, cap * sizeof(seq_t));
        if (!tmp) return 0;  /* allocation failure → undefined */
        cache = tmp;
    }

    /* Compute terms from (max_i + 1) up to n using the recurrence */
    for (idx_t i = max_i + 1; i <= n; ++i) {
        seq_t val =
            cache[i-1]
            + (seq_t)6 * cache[i-2]
            + (seq_t)10 * cache[i-3]
            + (seq_t)3 * cache[i-4]
            - (seq_t)13 * cache[i-5]
            + (seq_t)3 * cache[i-6]
            - (seq_t)3 * cache[i-7]
            - (seq_t)4 * cache[i-8]
            - cache[i-9];
        cache[i] = val;
    }
    max_i = n;
    return cache[n];
}
