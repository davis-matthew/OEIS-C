/*
 * A264300  Number of (1+1) X (n+1) arrays of permutations of 0..n*2+1 with
 *          each element having directed index change 0,1 0,-2 1,0 or -1,0.
 * https://oeis.org/A264300
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 35, u64 n <= 69, u128 n <= 139,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A264300
    Empirical recurrence:
        a(n) = a(n-1) + 3*a(n-3) + a(n-4) + a(n-5) 
               - 3*a(n-6) - 2*a(n-7) + a(n-9),
    with initial values:
        a(1)=1, a(2)=2, a(3)=5, a(4)=10, a(5)=16,
        a(6)=29, a(7)=61, a(8)=117, a(9)=212.
    The function supports all n ≥ 1, arbitrary order of calls,
    and caches previously computed values.
*/

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Return 0 for indices outside the defined range (not used in OEIS). */
    if (n < 1) return 0;

    /* Static cache of previously computed values. */
    static seq_t *cache   = NULL;   /* array of a(i) values */
    static idx_t cache_len = 0;     /* highest n that has been cached */

    /* Initialise cache with the first nine terms. */
    if (cache_len == 0) {
        cache_len = 9;
        cache = (seq_t *)calloc((size_t)(cache_len + 1), sizeof(seq_t));
        if (!cache) abort();        /* allocation failure */
        cache[1] = 1;  cache[2] = 2;  cache[3] = 5;
        cache[4] = 10; cache[5] = 16; cache[6] = 29;
        cache[7] = 61; cache[8] = 117; cache[9] = 212;
    }

    /* Extend cache if the requested index is beyond what we have. */
    if (n > cache_len) {
        seq_t *tmp = (seq_t *)realloc(cache,
                                      (size_t)(n + 1) * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;

        for (idx_t i = cache_len + 1; i <= n; ++i) {
            seq_t val = cache[i - 1];
            if (i >= 3) val += 3 * cache[i - 3];
            if (i >= 4) val +=       cache[i - 4];
            if (i >= 5) val +=       cache[i - 5];
            if (i >= 6) val -= 3 * cache[i - 6];
            if (i >= 7) val -= 2 * cache[i - 7];
            if (i >= 9) val +=       cache[i - 9];
            cache[i] = val;
        }
        cache_len = n;
    }

    return cache[(size_t)n];
}
