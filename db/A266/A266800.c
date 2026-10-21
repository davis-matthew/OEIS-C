/*
 * A266800  Coefficient of x in the minimal polynomial of the continued
 *          fraction [1^n,sqrt(3),1,1,...], where 1^n means n ones.
 * https://oeis.org/A266800
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 5*a(n-1) + 15*a(n-2) - 15*a(n-3) - 5*a(n-4) + a(n-5),
 *            a(0) = -8, a(1) = -12, a(2) = -98, a(3) = -636, a(4) = -4424
 * Range:     i8 n <= 2, i16 n <= 5, i32 n <= 10, i64 n <= 20, i128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static idx_t max_computed = (idx_t)-1;   /* no values stored yet */

    if (!cache) {
        const size_t init_cap = 8;
        cache = (seq_t*)malloc(init_cap * sizeof(seq_t));
        if (!cache) return 0;                /* allocation failure */
        cache[0] = -8;
        cache[1] = -12;
        cache[2] = -98;
        cache[3] = -636;
        cache[4] = -4424;
        cap = init_cap;
        max_computed = 4;
    }

    if (n <= max_computed) return cache[(size_t)n];

    if (n >= cap) {
        size_t new_cap = cap;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;                  /* allocation failure */
        cache = tmp;
        cap = new_cap;
    }

    for (idx_t i = max_computed + 1; i <= n; ++i) {
        size_t ii = (size_t)i;
        cache[ii] =
            5 * cache[ii-1] + 15 * cache[ii-2]
          - 15 * cache[ii-3] - 5 * cache[ii-4] + cache[ii-5];
    }
    max_computed = n;

    return cache[(size_t)n];
}
