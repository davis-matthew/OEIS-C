/*
 * A245955  Number of length 5+3 0..n arrays with some pair in every
 *          consecutive four terms totalling exactly n.
 * https://oeis.org/A245955
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 28, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: 
 *   a(n) = 2*a(n-1) + 4*a(n-2) - 10*a(n-3) - 5*a(n-4) + 20*a(n-5)
 *          - 20*a(n-7) + 5*a(n-8) + 10*a(n-9) - 4*a(n-10) - 2*a(n-11) + a(n-12)
 * Initial terms: a(1)=162, a(2)=4145, a(3)=26244, a(4)=128649, a(5)=381222,
 *                a(6)=1021225, a(7)=2217096, a(8)=4555697, a(9)=8345130,
 *                a(10)=14757441, a(11)=24276492, a(12)=38959225.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Memoization storage */
    static seq_t *cache = NULL;          /* array of computed values, 1-based */
    static idx_t cache_size = 0;         /* allocated size (including index 0) */
    static idx_t computed = 0;           /* largest n that has been computed */

    static const seq_t init[12] = {
        (seq_t)162,  (seq_t)4145,  (seq_t)26244,  (seq_t)128649,
        (seq_t)381222, (seq_t)1021225, (seq_t)2217096, (seq_t)4555697,
        (seq_t)8345130, (seq_t)14757441, (seq_t)24276492, (seq_t)38959225
    };

    /* Small n: return the seed value */
    if (n <= 12)
        return init[(size_t)n - 1];

    /* Allocate (or grow) cache so it can hold up to index n */
    if (n >= cache_size) {
        size_t new_cap = (size_t)n + 1;    /* need slot n (0..n) */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();                 /* allocation failed */
        cache = tmp;
        cache_size = new_cap;
    }

    /* Initialise the first 12 entries on first use */
    if (computed < 12) {
        for (size_t i = 1; i <= 12; ++i)
            cache[i] = init[i - 1];
        computed = 12;
    }

    /* Compute missing values up to n */
    for (idx_t i = computed + 1; i <= n; ++i) {
        seq_t val = 0;
        val += cache[i - 1] * (seq_t)2;
        val += cache[i - 2] * (seq_t)4;
        val -= cache[i - 3] * (seq_t)10;
        val -= cache[i - 4] * (seq_t)5;
        val += cache[i - 5] * (seq_t)20;
        val -= cache[i - 7] * (seq_t)20;
        val += cache[i - 8] * (seq_t)5;
        val += cache[i - 9] * (seq_t)10;
        val -= cache[i -10] * (seq_t)4;
        val -= cache[i -11] * (seq_t)2;
        val += cache[i -12];               /* times 1 */

        cache[i] = val;
    }

    computed = n;
    return cache[n];
}
