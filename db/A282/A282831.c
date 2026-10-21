/*
 * A282831  Number of nX1 0..1 arrays with no 1 equal to more than one of its
 *          king-move neighbors, with the exception of exactly two elements.
 * https://oeis.org/A282831
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 34, u64 n <= 69, u128 n <= 139,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Formula (empirical recurrence):
   a(n) = 3*a(n-1) - 2*a(n-3) - 6*a(n-4) + 4*a(n-6) + 6*a(n-7) + 3*a(n-8) + a(n-9)
   with initial values:
   a(1)=0, a(2)=0, a(3)=0, a(4)=1, a(5)=2, a(6)=5,
   a(7)=13, a(8)=29, a(9)=65
*/

#include <stdlib.h>

static seq_t *cache = NULL;          /* cache[i] holds a(i) for i >= 1 */
static idx_t  cached_until = 0;     /* highest n for which a(n) is computed */

/* Base values for n = 1..9 (index 0 unused) */
static const seq_t base[10] = { 0, 0, 0, 0, 1, 2, 5, 13, 29, 65 };

seq_t a(idx_t n)
{
    /* Handle the offset: a(1) is the first defined term */
    if (n <= 9) {
        return base[(size_t)n];
    }

    /* Initialise or extend the cache to contain at least n+1 slots */
    if (cache == NULL) {
        size_t init_size = (size_t)n + 1;
        cache = (seq_t *)malloc(init_size * sizeof(seq_t));
        if (cache == NULL) return 0;                /* allocation failure */

        cache[0] = 0;                               /* dummy slot */
        for (idx_t i = 1; i <= 9; i++) cache[i] = base[i];
        cached_until = 9;
    }
    else if (n > cached_until) {
        size_t new_size = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (tmp == NULL) return 0;                 /* reallocation failure */
        cache = tmp;
        /* No need to initialise the new portion; it will be filled below */
    }

    /* Compute any missing terms up to n */
    for (idx_t i = cached_until + 1; i <= n; i++) {
        /* All intermediate calculations are performed in seq_t arithmetic */
        seq_t pos = 3 * cache[i - 1];
        pos += 4 * cache[i - 6];
        pos += 6 * cache[i - 7];
        pos += 3 * cache[i - 8];
        pos += cache[i - 9];

        seq_t neg = 2 * cache[i - 3] + 6 * cache[i - 4];
        cache[i] = pos - neg;
    }

    cached_until = n;
    return cache[n];
}
