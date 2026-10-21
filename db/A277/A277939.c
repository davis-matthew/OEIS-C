/*
 * A277939  Number of n X 2 0..2 arrays with every element plus 1 mod 3 equal
 *          to some element at offset (-1,0) (-1,1) (0,-1) (0,1) or (1,0),
 *          with upper left element zero.
 * https://oeis.org/A277939
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) + 6*a(n-2) - a(n-3) - 2*a(n-4) with a(1)=0,
 *            a(2)=3, a(3)=14, a(4)=74 (OEIS A277939)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 14, u64 n <= 28, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* static cache: values computed so far */
    static seq_t *cache = NULL;
    static size_t cap     = 0;   /* allocated size of cache (in seq_t elements) */
    static idx_t  up_to   = 0;   /* largest n for which cache[n] is defined */

    if (n < 1)
        return (seq_t)0;   /* out of range – not used in OEIS */

    /* ensure cache large enough for index n */
    if (cap <= n) {
        size_t new_cap = (cap == 0) ? (n + 1) : (cap << 1);
        while (new_cap <= n)
            new_cap <<= 1;
        seq_t *tmp = (seq_t*)malloc(new_cap * sizeof(seq_t));
        if (!tmp) {
            /* allocation failed – fallback to recomputing without cache */
            up_to = 0;
            cap   = 0;
            cache = NULL;
        } else {
            if (cache) {
                memcpy(tmp, cache, (up_to + 1) * sizeof(seq_t));
                free(cache);
            }
            cache = tmp;
            cap   = new_cap;
        }
    }

    /* compute values up to n */
    for (idx_t i = up_to + 1; i <= n; ++i) {
        if (i == 1)
            cache[i] = (seq_t)0;
        else if (i == 2)
            cache[i] = (seq_t)3;
        else if (i == 3)
            cache[i] = (seq_t)14;
        else if (i == 4)
            cache[i] = (seq_t)74;
        else
            cache[i] = (seq_t)( 4 * cache[i-1]
                               + 6 * cache[i-2]
                               -   cache[i-3]
                               - 2 * cache[i-4] );
    }
    up_to = n;

    return cache[n];
}
