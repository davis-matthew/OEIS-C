/*
 * A345953  Number of ways to tile a 2 X n strip with squares and P-shaped
 *          pentominos.
 * https://oeis.org/A345953
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 31, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-1) + 4*a(n-3) + 2*a(n-5)
   with initial values
   a(0)=1, a(1)=1, a(2)=1, a(3)=5, a(4)=9, a(5)=15 */
#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;      /* stored values */
    static idx_t  sz     = 0;        /* current size of cache */

    if (n < sz)
        return cache[n];

    /* expand cache to hold indices up to n */
    seq_t *tmp = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
    if (!tmp)
        return 0; /* memory allocation failed – caller will treat as 0 */
    cache = tmp;

    for (idx_t i = sz; i <= n; ++i) {
        if (i == 0)
            cache[i] = 1;
        else if (i == 1)
            cache[i] = 1;
        else if (i == 2)
            cache[i] = 1;
        else if (i == 3)
            cache[i] = 5;
        else if (i == 4)
            cache[i] = 9;
        else if (i == 5)
            cache[i] = 15;
        else {
            seq_t term = cache[i - 1]
                       + ((seq_t)4 * cache[i - 3])
                       + ((seq_t)2 * cache[i - 5]);
            cache[i] = term;
        }
    }
    sz = n + 1;
    return cache[n];
}
