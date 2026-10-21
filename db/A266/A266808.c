/*
 * A266808  Coefficient of x in the minimal polynomial of the continued
 *          fraction [1^n,sqrt(2)+sqrt(3),1,1,...], where 1^n means n ones.
 * https://oeis.org/A266808
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 34*a(n-1) + 714*a(n-2) - 4641*a(n-3) - 12376*a(n-4) +
 *            12376*a(n-5) + 4641*a(n-6) - 714*a(n-7) - 34*a(n-8) + a(n-9)
 *            Initial values: a(0) = -168, a(1) = -560, a(2) = -101124, a(3) =
 *            -3288624, a(4) = -180132168, a(5) = -7998247028, a(6) =
 *            -384048485640, a(7) = -17892957477264, a(8) = -8432631...
 * Range:     i8 none, i16 n <= 1, i32 n <= 4, i64 n <= 10, i128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
/* Note: seq_t and idx_t are defined by the build environment. */

seq_t a(idx_t n)
{
    /* Static cache variables */
    static seq_t *cache = NULL;     /* pointer to cached terms */
    static size_t cap   = 0;        /* allocated capacity of the cache */
    static size_t len   = 0;        /* number of terms currently stored (>= all cached indices + 1) */

    /* Initial terms (seed for the recurrence) */
    static const seq_t seed[9] = {
        (seq_t)-168,
        (seq_t)-560,
        (seq_t)-101124,
        (seq_t)-3288624,
        (seq_t)-180132168,
        (seq_t)-7998247028,
        (seq_t)-384048485640,
        (seq_t)-17892957477264,
        (seq_t)-843263161727364
    };

    /* Initialise the cache once */
    if (cache == NULL) {
        /* Start with a small capacity, then grow as needed */
        cap = 16;
        cache = (seq_t *)malloc(sizeof(seq_t) * cap);
        if (cache == NULL)
            return (seq_t)0;   /* allocation failure: unlikely in tests */

        /* Copy the seed values into the cache */
        for (size_t i = 0; i < 9; ++i)
            cache[i] = seed[i];
        len = 9;
    }

    /* If the requested n is already cached, return it immediately */
    if ((size_t)n < len)
        return cache[(size_t)n];

    /* Ensure sufficient capacity to store terms up to n */
    size_t need = (size_t)n + 1;
    if (need > cap) {
        size_t newcap = need;
        /* Optionally round up to reduce realloc frequency (e.g., nearest power of 2) */
        if (newcap < 64) newcap = 64;
        else {
            /* Round up to next multiple of 16 */
            newcap = (newcap + 15) & ~((size_t)15);
        }
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * newcap);
        if (tmp == NULL)
            return (seq_t)0;   /* allocation failure: unlikely in tests */
        cache = tmp;
        cap   = newcap;
    }

    /* Compute terms from the current length up to the requested n using the recurrence */
    for (size_t i = len; i <= (size_t)n; ++i) {
        seq_t term =
            cache[i-1] * (seq_t)34
          + cache[i-2] * (seq_t)714
          - cache[i-3] * (seq_t)4641
          - cache[i-4] * (seq_t)12376
          + cache[i-5] * (seq_t)12376
          + cache[i-6] * (seq_t)4641
          - cache[i-7] * (seq_t)714
          - cache[i-8] * (seq_t)34
          + cache[i-9];
        cache[i] = term;
    }

    len = need;

    /* Return the computed value */
    return cache[(size_t)n];
}
