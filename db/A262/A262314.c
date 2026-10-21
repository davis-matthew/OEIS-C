/*
 * A262314  Number of (n+2)X(2+2) 0..1 arrays with each row and column
 *          divisible by 7, read as a binary number with top and left being
 *          the most significant bits.
 * https://oeis.org/A262314
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 21, u64 n <= 41, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = 4*a(n-1) -4*a(n-2) +20*a(n-3) -68*a(n-4) +68*a(n-5) 
 *               -162*a(n-6) +444*a(n-7) -444*a(n-8) +644*a(n-9) 
 *               -1244*a(n-10) +1244*a(n-11) -1277*a(n-12) +1376*a(n-13) 
 *               -1376*a(n-14) +1032*a(n-15)
 * Initial values: a(1)=3, a(2)=5, a(3)=9, a(4)=27, a(5)=61, a(6)=145, 
 *                 a(7)=435, a(8)=1253, a(9)=3593, a(10)=10779, a(11)=32445, 
 *                 a(12)=96881, a(13)=290643, a(14)=875365, a(15)=2627721
 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Constants */
    static const seq_t first15[15] = {
        3, 5, 9, 27, 61, 145, 435, 1253, 3593, 10779,
        32445, 96881, 290643, 875365, 2627721
    };
    static const int coeffs[15] = {
        4, -4, 20, -68, 68, -162, 444, -444, 644, -1244,
        1244, -1277, 1376, -1376, 1032
    };

    /* Memoization buffer */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;  /* largest computed index + 1 (1-based) */

    if (n <= 0) {
        /* Undefined for n <= 0; return 0 as a fallback */
        return (seq_t)0;
    }

    /* Ensure cache has space for index n */
    if (cache_len <= n) {
        idx_t new_len = (n > 1024) ? (n + 1) : 1025; /* avoid too many reallocs */
        if (cache == NULL) {
            cache = (seq_t *)malloc(new_len * sizeof(seq_t));
            if (!cache) return (seq_t)0;  /* allocation failure */
            /* initialise first element as dummy */
            cache[0] = (seq_t)0;
        } else {
            seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
            if (!tmp) return (seq_t)0;  /* reallocation failure */
            cache = tmp;
        }
        /* Initialise new entries */
        for (idx_t i = cache_len; i <= n; ++i) {
            if (i <= 15) {
                cache[i] = first15[i - 1];
            } else {
                seq_t sum = (seq_t)0;
                for (int k = 0; k < 15; ++k) {
                    int c = coeffs[k];
                    seq_t term = cache[i - 1 - k] * (seq_t)((c >= 0) ? c : -c);
                    if (c < 0)
                        sum -= term;
                    else
                        sum += term;
                }
                cache[i] = sum;
            }
        }
        cache_len = n + 1;
    }

    return cache[n];
}
