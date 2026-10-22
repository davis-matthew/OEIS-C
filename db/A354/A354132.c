/*
 * A354132  Number of tilings of a 3 X n rectangle using 2 X 2 and 1 X 1
 *          tiles, right trominoes and dominoes.
 * https://oeis.org/A354132
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) + 38*a(n-2) - 68*a(n-4) + 24*a(n-5) - 3*a(n-6)
 *            Initial values: a(0)=1, a(1)=3, a(2)=48, a(3)=405, a(4)=4185,
 *            a(5)=40320
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* cached terms */
    static size_t cap = 0;       /* number of allocated slots */
    static idx_t upto = 0;        /* highest index computed so far */

    /* first allocation */
    if (cache == NULL) {
        cap = 16;
        cache = (seq_t *)calloc(cap, sizeof(seq_t));
        if (cache == NULL) return 0;          /* allocation failure not expected */
    }

    /* ensure array large enough for n */
    while ((size_t)n + 1 > cap) {
        size_t newcap = cap * 2;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (tmp == NULL) return 0;          /* allocation failure not expected */
        cache = tmp;
        cap = newcap;
    }

    /* initialise first six terms once */
    if (upto < 5) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)48;
        cache[3] = (seq_t)405;
        cache[4] = (seq_t)4185;
        cache[5] = (seq_t)40320;
        upto = 5;
    }

    if (n <= upto)
        return cache[(size_t)n];

    /* compute missing terms on demand */
    for (idx_t k = upto + 1; k <= n; ++k) {
        seq_t term = (seq_t)6 * cache[(size_t)(k - 1)]
                   + (seq_t)38 * cache[(size_t)(k - 2)]
                   + (seq_t)24 * cache[(size_t)(k - 5)]
                   - (seq_t)68 * cache[(size_t)(k - 4)]
                   - (seq_t)3  * cache[(size_t)(k - 6)];
        cache[(size_t)k] = term;
    }

    upto = n;
    return cache[(size_t)n];
}
