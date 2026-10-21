/*
 * A252311  Number of (6+2)X(n+2) 0..3 arrays with every 3X3 subblock row and
 *          diagonal sum equal to 0 1 3 6 or 7 and every 3X3 column and
 *          antidiagonal sum not equal to 0 1 3 6 or 7.
 * https://oeis.org/A252311
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 15*a(n-3) -70*a(n-6) +120*a(n-9) -64*a(n-12) (for n>21) *
 *            Initial values: * a(1)=3681, a(2)=3476, a(3)=5416, a(4)=7489,
 *            a(5)=14515, * a(6)=24076, a(7)=34279, a(8)=70643, a(9)=120328,
 *            a(10)=173799, * a(11)=386803, a(12)=679072, a(13)=998407,
 *            a(14)=2392371, * a(15)=4310288, a(16)=6431079, a(...
 * Range:     u8 none, u16 n <= 7, u32 n <= 25, u64 n <= 57, u128 n <= 121,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init[22] = {
    0ULL,                /* index 0 unused */
    3681ULL, 3476ULL, 5416ULL, 7489ULL, 14515ULL,
    24076ULL, 34279ULL, 70643ULL, 120328ULL, 173799ULL,
    386803ULL, 679072ULL, 998407ULL, 2392371ULL, 4310288ULL,
    6431079ULL, 16347187ULL, 30013936ULL, 45246631ULL,
    119642163ULL, 222278576ULL
};

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_n = 0;

    if (n <= 0) {
        return (seq_t)0;
    }

    /* Allocate or grow cache to hold up to n */
    if (cache == NULL || n > cache_n) {
        idx_t newsize = n + 1;                /* +1 to use 1‑based indexing */
        cache = (seq_t *)realloc(cache, sizeof(seq_t) * newsize);
        if (cache == NULL) {
            /* Allocation failed; return 0 as a safe fallback */
            return (seq_t)0;
        }

        /* Fill newly required entries */
        for (idx_t i = cache_n + 1; i <= n; ++i) {
            if (i <= 21) {
                cache[i] = init[i];
            } else {
                /* Recurrence: a[n] = 15*a[n-3] - 70*a[n-6] + 120*a[n-9] - 64*a[n-12] */
                cache[i] = 15ULL * cache[i - 3]
                           - 70ULL * cache[i - 6]
                           + 120ULL * cache[i - 9]
                           - 64ULL * cache[i - 12];
            }
        }
        cache_n = n;
    }

    return cache[n];
}
