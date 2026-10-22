/*
 * A306048  Number of nX3 0..1 arrays with every element unequal to 0, 1, 2,
 *          3, 6 or 7 king-move adjacent elements, with upper left element
 *          zero.
 * https://oeis.org/A306048
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 39, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 3*a(n-1) + 9*a(n-3) - 20*a(n-4) - 27*a(n-5)
 *                + 11*a(n-6) + 33*a(n-7) + 18*a(n-8)
 *                - 22*a(n-9) - 10*a(n-10)  for n > 12
 *  Initial values (1‑based index):
 *    a(1)=4,  a(2)=21,  a(3)=45,  a(4)=142,  a(5)=439,
 *    a(6)=1261, a(7)=3826, a(8)=11770, a(9)=35152, a(10)=106027,
 *    a(11)=323339, a(12)=977445
 */

#include <stdlib.h>     /* for malloc, realloc, free */
#include <stddef.h>     /* for size_t */

seq_t a(idx_t n)
{
    /* memoization storage */
    static seq_t *memo   = NULL;
    static size_t cap    = 0;          /* allocated slots (>= n+1) */
    static idx_t  max_n  = 0;          /* highest n computed so far */

    /* initial 12 values (index 0 unused) */
    static const seq_t init[13] = {
        0ULL,            /* dummy at index 0 */
        4ULL, 21ULL, 45ULL, 142ULL, 439ULL,
        1261ULL, 3826ULL, 11770ULL, 35152ULL, 106027ULL,
        323339ULL, 977445ULL
    };

    if (n < 1)
        return (seq_t)0;   /* undefined for n < 1 */

    /* allocate or grow memo array if necessary */
    if (n >= (idx_t)cap) {
        size_t new_cap = cap ? cap * 2 : 16;
        if (new_cap <= (size_t)n) new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(memo, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;   /* allocation failure: return 0 */
        memo = tmp;
        cap  = new_cap;
    }

    /* initialize memo with the 12 initial values on first use */
    if (max_n == 0) {
        for (idx_t i = 1; i <= 12; ++i) memo[i] = init[i];
        max_n = 12;
    }

    /* for any n <= 12, the value is already set */
    if (n <= 12) return memo[n];

    /* extend memo to cover requested n */
    for (idx_t i = max_n + 1; i <= n; ++i) {
        seq_t v = 0;
        /* use unsigned arithmetic; overflow is modulo 2^N as needed */
        v += (seq_t)3  * memo[i - 1];
        v += (seq_t)9  * memo[i - 3];
        v -= (seq_t)20 * memo[i - 4];
        v -= (seq_t)27 * memo[i - 5];
        v += (seq_t)11 * memo[i - 6];
        v += (seq_t)33 * memo[i - 7];
        v += (seq_t)18 * memo[i - 8];
        v -= (seq_t)22 * memo[i - 9];
        v -= (seq_t)10 * memo[i -10];
        memo[i] = v;
    }
    max_n = n;

    return memo[n];
}
