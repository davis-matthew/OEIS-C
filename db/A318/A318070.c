/*
 * A318070  Number of nX3 0..1 arrays with every element unequal to 0, 1, 2 or
 *          5 horizontally, vertically or antidiagonally adjacent elements,
 *          with upper left element zero.
 * https://oeis.org/A318070
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n‑1) + a(n‑2) + a(n‑3) - 3*a(n‑4) - 6*a(n‑5) +
 *            6*a(n‑6) (valid for n>10). Initial values: a(1)=4, a(2)=10,
 *            a(3)=18, a(4)=41, a(5)=81, a(6)=179, a(7)=404, a(8)=893,
 *            a(9)=2000, a(10)=4516
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 26, u64 n <= 54, u128 n <= 109,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Our cache stores a(0) as unused, indices [0..max] */
    static size_t       cache_cap = 0;
    static seq_t *      cache     = NULL;
    static idx_t        max_comp  = 0;   /* largest n whose value is stored */

    if (n < 1) return 0;   /* undefined for n<1; return 0 as sentinel */

    /* Grow the cache array until it can address index n */
    if (n >= (idx_t)cache_cap) {
        size_t new_cap = cache_cap ? (cache_cap << 1) : 16;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;   /* allocation failed */
        cache      = tmp;
        cache_cap  = new_cap;
    }

    /* Initialise the first ten terms if not yet done */
    if (max_comp < 10) {
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)4;
        cache[2] = (seq_t)10;
        cache[3] = (seq_t)18;
        cache[4] = (seq_t)41;
        cache[5] = (seq_t)81;
        cache[6] = (seq_t)179;
        cache[7] = (seq_t)404;
        cache[8] = (seq_t)893;
        cache[9] = (seq_t)2000;
        cache[10] = (seq_t)4516;
        max_comp = 10;
    }

    /* Extend the cache using the recurrence when necessary */
    if (n > max_comp) {
        for (idx_t i = max_comp + 1; i <= n; ++i) {
            /* The recurrence requires at least n>10, but
               the loop begins strictly above max_comp,
               which is ≥10, so i≥11 here. */
            seq_t temp = cache[i-1];
            temp = temp * (seq_t)2;
            temp += cache[i-2];
            temp += cache[i-3];
            temp -= cache[i-4] * (seq_t)3;
            temp -= cache[i-5] * (seq_t)6;
            temp += cache[i-6] * (seq_t)6;
            cache[i] = temp;
        }
        if (n > max_comp) max_comp = n;
    }

    return cache[n];
}
