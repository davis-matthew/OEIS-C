/*
 * A259243  Number of (n+1) X (1+1) 0..1 arrays with each 2 X 2 subblock
 *          having clockwise pattern 0000 0011 or 0111.
 * https://oeis.org/A259243
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 9, a(2) = 21, a(n) = a(n-1) + 3*a(n-2) (n ≥ 3)
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 24, u64 n <= 51, u128 n <= 104,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cap   = 0;
    static idx_t last  = 0;

    if (n < 1) return 0;                 /* undefined for n < 1 */

    /* ensure storage for indices up to n */
    if (n >= cap) {
        idx_t new_cap = cap ? cap * 2 : 2;
        while (new_cap <= n) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, (new_cap + 1) * sizeof(seq_t));
        cache = tmp;
        cap   = new_cap;
    }

    /* compute missing values up to n */
    for (idx_t i = last + 1; i <= n; ++i) {
        if (i == 1)              cache[1] = (seq_t)9;
        else if (i == 2)         cache[2] = (seq_t)21;
        else                     cache[i] = cache[i - 1] + (seq_t)3 * cache[i - 2];
    }
    last = n;
    return cache[n];
}
