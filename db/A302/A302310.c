/*
 * A302310  Number of 2Xn 0..1 arrays with every element equal to 0, 1, 3 or 4
 *          horizontally, diagonally or antidiagonally adjacent elements, with
 *          upper left element zero.
 * https://oeis.org/A302310
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 24, u64 n <= 48, u128 n <= 95,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A302310: Number of 2×n 0..1 arrays with adjacency constraints
   Recurrence for n > 6:
       a(n) = 2*a(n-1) - a(n-2) + 4*a(n-3) + 12*a(n-4) - 16*a(n-5)
   Initial values:
       a(1)=2, a(2)=3, a(3)=11, a(4)=21, a(5)=31, a(6)=113
*/

#include <stdlib.h>

static const seq_t init_values[7] = {0, 2, 3, 11, 21, 31, 113};
static seq_t *cache   = NULL;      /* dynamically allocated cache */
static size_t cache_size   = 0;     /* allocated array length */
static size_t computed_up_to = 6;   /* highest index computed so far */

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* undefined, return 0 */

    if (n <= 6)                           /* use initial values directly */
        return init_values[n];

    /* Ensure the cache can address indices 0..n */
    size_t need = (size_t)n + 1;
    if (cache_size < need) {
        seq_t *tmp = (seq_t*)realloc(cache, need * sizeof(seq_t));
        if (!tmp) abort();               /* memory allocation failure */
        cache      = tmp;
        cache_size = need;
    }

    /* (Re)initialize the known initial values in the cache */
    for (size_t i = 1; i <= 6; ++i)
        cache[i] = init_values[i];

    /* Compute missing terms up to n */
    if ((size_t)n > computed_up_to) {
        for (size_t i = computed_up_to + 1; i <= (size_t)n; ++i) {
            cache[i] =
                ((seq_t)2) * cache[i-1]
                - cache[i-2]
                + ((seq_t)4) * cache[i-3]
                + ((seq_t)12) * cache[i-4]
                - ((seq_t)16) * cache[i-5];
        }
        computed_up_to = (size_t)n;
    }

    return cache[(size_t)n];
}
