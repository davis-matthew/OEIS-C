/*
 * A269271  Number of n X 3 0..3 arrays with some element plus some
 *          horizontally or antidiagonally adjacent neighbor totalling three
 *          exactly once.
 * https://oeis.org/A269271
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 42*a(n-1) - 441*a(n-2); a(1)=24, a(2)=1368
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n <= 0) return 0;          /* undefined for n < 1, return 0 */
    if (n == 1) return 24;         /* base case */
    if (n == 2) return 1368;       /* base case */

    /* static memoization of computed values */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    if (!cache) {                  /* initialise for first two terms */
        cache_size = 2;
        cache = (seq_t*)malloc(sizeof(seq_t) * 2);
        cache[0] = 24;
        cache[1] = 1368;
    }

    while (cache_size < n) {
        seq_t a_nm1 = cache[cache_size - 1];   /* a(k-1) */
        seq_t a_nm2 = cache[cache_size - 2];   /* a(k-2) */
        seq_t val = a_nm1 * (seq_t)42U - a_nm2 * (seq_t)441U;
        cache = (seq_t*)realloc(cache, sizeof(seq_t) * (cache_size + 1));
        cache[cache_size] = val;
        cache_size++;
    }
    return cache[n - 1];
}
