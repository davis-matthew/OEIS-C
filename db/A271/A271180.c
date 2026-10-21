/*
 * A271180  Expansion of
 *          (4*x^3-7*x^2+4*x-1)/(x^6-4*x^5+4*x^4+x^3-7*x^2+5*x-1).
 * https://oeis.org/A271180
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1) - 7*a(n-2) + a(n-3) + 4*a(n-4) - 4*a(n-5) +
 *            a(n-6) for n > 5; a(0)=1, a(1)=1, a(2)=5, a(3)=15, a(4)=45,
 *            a(5)=125 (Offset: 0)
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 22, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static idx_t  last  = (idx_t)-1;   /* index of last filled term */

    /* initialise cache and base values once */
    if (last == (idx_t)-1) {
        /* start with a small capacity that covers the base terms */
        size_t init_cap = 16;
        cache = (seq_t *)malloc(init_cap * sizeof(seq_t));
        cap   = init_cap;

        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)5;
        cache[3] = (seq_t)15;
        cache[4] = (seq_t)45;
        cache[5] = (seq_t)125;
        last   = 5;
    }

    /* enlarge cache if needed */
    if ((size_t)n >= cap) {
        size_t new_cap = (size_t)n + 1;
        if (new_cap < cap * 2) new_cap = cap * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        /* in a robust implementation, check for tmp==NULL; omitted for brevity */
        cache = tmp;
        cap   = new_cap;
    }

    /* return already memoised value */
    if (n <= last)
        return cache[n];

    /* compute missing terms up to n */
    for (idx_t i = last + 1; i <= n; ++i) {
        seq_t term =
            (seq_t)5 * cache[i-1]
          - (seq_t)7 * cache[i-2]
          +       cache[i-3]
          + (seq_t)4 * cache[i-4]
          - (seq_t)4 * cache[i-5]
          +       cache[i-6];
        cache[i] = term;
    }
    last = n;
    return cache[n];
}
