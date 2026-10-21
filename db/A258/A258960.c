/*
 * A258960  Number of (n+2) X (2+2) 0..1 arrays with no 3 X 3 subblock
 *          diagonal sum 0 and no antidiagonal sum 3 and no row sum 0 or 3 and
 *          no column sum 0 or 3.
 * https://oeis.org/A258960
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Empirical recurrence a(n) = a(n-1) + a(n-2) - a(n-3) + 4*a(n-4)
 *            - 4*a(n-5) - 3*a(n-6) + 3*a(n-7) for n > 13, with initial terms:
 *            a(1)=158, a(2)=344, a(3)=274, a(4)=467, a(5)=614, a(6)=951,
 *            a(7)=1228, a(8)=1951, a(9)=2652, a(10)=4261, a(11)=5688,
 *            a(12)=9170, a(13)=12562
 * Range:     u8 n <= 1, u16 n <= 17, u32 n <= 45, u64 n <= 101, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;          /* behaviour below the offset is irrelevant */

    /* Constants for a(1)..a(13) */
    static const seq_t base[14] = {
        0,            /* unused index 0 */
        158, 344, 274, 467, 614, 951, 1228, 1951,
        2652, 4261, 5688, 9170, 12562
    };

    if (n <= 13)
        return base[n];

    /* Persistent cache for computed terms */
    static seq_t *cache = NULL;
    static size_t cap = 0;          /* allocated size (including index 0) */
    static idx_t max_computed = 0;  /* highest n that has been computed */

    /* Allocate or enlarge cache if required */
    if (cache == NULL || cap <= n) {
        size_t new_cap = cap == 0 ? 16 : cap;
        while (new_cap <= n)
            new_cap <<= 1;          /* grow geometrically */

        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL)          /* allocation failed – give up */
            return 0;
        cache = tmp;
        cap = new_cap;
    }

    /* Initialise the base values once */
    if (max_computed < 13) {
        for (idx_t i = max_computed + 1; i <= 13; ++i)
            cache[i] = base[i];
        max_computed = 13;
    }

    /* Compute missing terms using the recurrence */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        cache[i] =
            cache[i-1] +
            cache[i-2] -
            cache[i-3] +
            (seq_t)4 * cache[i-4] -
            (seq_t)4 * cache[i-5] -
            (seq_t)3 * cache[i-6] +
            (seq_t)3 * cache[i-7];
    }
    max_computed = n;

    return cache[n];
}
