/*
 * A270167  First differences of number of active (ON, black) cells in n-th
 *          stage of growth of two-dimensional cellular automaton defined by
 *          "Rule 107", based on the 5-celled von Neumann neighborhood.
 * https://oeis.org/A270167
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -a(n-1)+2*a(n-2)+2*a(n-3)-a(n-4)-a(n-5) (n>12)
 * Range:     i8 n <= 5, i16 n <= 103, i32 n <= 127, i64 n <= 127,
 *            i128 n <= 127, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Initial values: a(0)=4, a(1)=0, a(2)=36, a(3)=-29, a(4)=97, a(5)=-93, */
/*                 a(6)=181, a(7)=-157, a(8)=281, a(9)=-261, a(10)=409, */
/*                 a(11)=-385, a(12)=561 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Cache of previously computed values */
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static size_t len = 0;

    /* Convert idx_t to size_t safely */
    size_t idx = (size_t)n;

    /* Base terms up to n=12 */
    static const seq_t base[13] = {
        4,   0, 36, -29, 97,
       -93, 181, -157, 281, -261,
       409, -385, 561
    };

    /* Initialize cache if first call */
    if (cache == NULL) {
        cap = 13;                 /* capacity for indices 0..12 */
        len = 13;                 /* length of initialized data */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (cache == NULL) return 0;      /* out of memory; abort gracefully */
        for (size_t i = 0; i < 13; ++i) cache[i] = base[i];
    }

    /* Grow cache if needed */
    if (idx >= cap) {
        size_t new_cap = cap;
        while (new_cap <= idx) {
            new_cap = (new_cap == 0) ? 1 : new_cap * 2;
        }
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return 0;          /* memory failure */
        cache = tmp;
        cap = new_cap;
    }

    /* Compute additional values up to idx */
    if (idx >= len) {
        for (size_t i = len; i <= idx; ++i) {
            seq_t a1 = cache[i - 1];
            seq_t a2 = cache[i - 2];
            seq_t a3 = cache[i - 3];
            seq_t a4 = cache[i - 4];
            seq_t a5 = cache[i - 5];
            cache[i] = -a1 + 2 * a2 + 2 * a3 - a4 - a5;
        }
        len = idx + 1;
    }

    return cache[idx];
}
