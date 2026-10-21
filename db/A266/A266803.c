/*
 * A266803  Coefficient of x^0 in the minimal polynomial of the continued
 *          fraction [1^n,sqrt(2)+sqrt(3),1,1,...], where 1^n means n ones.
 * https://oeis.org/A266803
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 34*a(n-1) + 714*a(n-2) - 4641*a(n-3) - 12376*a(n-4) +
 *            12376*a(n-5) + 4641*a(n-6) - 714*a(n-7) - 34*a(n-8) + a(n-9)
 *            Initial values: a(0) = 49, a(1) = 49, a(2) = 25281, a(3) =
 *            606409, a(4) = 37676521, a(5) = 1596669889, a(6) = 78061422609,
 *            a(7) = 3612062087761, a(8) = 170677159358209
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 5, i64 n <= 10, i128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t  cap    = 0;   /* number of elements currently allocated */
    static idx_t  last   = 8;   /* highest index already computed */
    static bool   init   = false;

    /* initialise cache on first call with the first 9 values */
    if (!init) {
        static const seq_t init_vals[9] = {
            49L, 49L, 25281L, 606409L,
            37676521L, 1596669889L, 78061422609L,
            3612062087761L, 170677159358209L
        };
        cap  = 9;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return 0; /* allocation failure – return 0 as placeholder */
        for (idx_t i = 0; i < cap; ++i)
            cache[i] = init_vals[i];
        init = true;
    }

    /* return cached value if already computed */
    if (n <= last)
        return cache[n];

    /* expand cache to allow storage up to index n */
    if (n >= cap) {
        idx_t newcap = cap ? cap * 2 : 1;
        while (newcap <= n)
            newcap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) return 0;  /* allocation failure */
        cache = tmp;
        cap   = newcap;
    }

    /* compute missing terms using the recurrence */
    for (idx_t i = last + 1; i <= n; ++i) {
        seq_t v = 34 * cache[i-1];
        v += 714 * cache[i-2];
        v -= 4641 * cache[i-3];
        v -= 12376 * cache[i-4];
        v += 12376 * cache[i-5];
        v += 4641 * cache[i-6];
        v -= 714 * cache[i-7];
        v -= 34 * cache[i-8];
        v += cache[i-9];
        cache[i] = v;
    }

    last = n;
    return cache[n];
}
