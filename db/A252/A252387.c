/*
 * A252387  Number of (3+2) X (n+2) 0..3 arrays with every 3 X 3 subblock row
 *          and diagonal sum equal to 0 3 5 6 or 7 and every 3 X 3 column and
 *          antidiagonal sum not equal to 0 3 5 6 or 7.
 * https://oeis.org/A252387
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 18, u32 n <= 53, u64 n <= 122, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(n) = 4*a(n-3) - 4*a(n-6) + a(n-9)   for n > 17
   Initial values:
     a(1)=588, a(2)=529, a(3)=632, a(4)=663, a(5)=963,
     a(6)=1341, a(7)=1610, a(8)=2376, a(9)=3331,
     a(10)=4097, a(11)=6077, a(12)=8560, a(13)=10605,
     a(14)=15764, a(15)=22252, a(16)=27640, a(17)=41123
   The function memoises results so that repeated queries are O(1).  */

#include <stdlib.h>

static const seq_t _A252387_init[17] = {
    588, 529, 632, 663, 963,
    1341, 1610, 2376, 3331,
    4097, 6077, 8560, 10605,
    15764, 22252, 27640, 41123
};

seq_t a(idx_t n)
{
    if (n <= 17)           /* return the predefined initial term */
        return _A252387_init[n - 1];

    /* Memoised cache of computed values */
    static seq_t *cache = NULL;
    static idx_t  cache_max = 0;        /* largest index currently stored */

    /* Initialise cache with the first 17 terms */
    if (cache == NULL) {
        cache = (seq_t *)malloc(sizeof(seq_t) * (18)); /* index 0 unused */
        if (cache == NULL) return 0; /* allocation failure -- unlikely */
        for (idx_t i = 1; i <= 17; ++i)
            cache[i] = _A252387_init[i - 1];
        cache_max = 17;
    }

    /* Grow cache if the requested index is beyond what we have */
    if (n > cache_max) {
        seq_t *new_cache = (seq_t *)realloc(cache, sizeof(seq_t) * (n + 1));
        if (new_cache == NULL) return 0; /* allocation failure -- unlikely */
        cache = new_cache;
        for (idx_t i = cache_max + 1; i <= n; ++i) {
            /* recurrence: a(i) = 4*a(i-3) - 4*a(i-6) + a(i-9) */
            seq_t val = 4 * cache[i - 3] - 4 * cache[i - 6] + cache[i - 9];
            cache[i] = val;
        }
        cache_max = n;
    }

    return cache[n];
}
