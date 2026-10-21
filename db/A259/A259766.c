/*
 * A259766  Number of (n+2) X (2+2) 0..1 arrays with each 3 X 3 subblock
 *          having clockwise perimeter pattern 00000001 or 00000101.
 * https://oeis.org/A259766
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) =
 *            a(n-1)+a(n-2)+a(n-3)+2*a(n-4)-2*a(n-5)+2*a(n-6)-4*a(n-7)-3*a(n-8)+2*a(n-9)-4*a(n-10)+2*a(n-11)
 *            with initial terms: a(1)=49, a(2)=87, a(3)=167, a(4)=299,
 *            a(5)=564, a(6)=1086, a(7)=2045, a(8)=3870, a(9)=7371,
 *            a(10)=14001, a(11)=26647, a(12)=50766
 * Range:     u8 n <= 3, u16 n <= 12, u32 n <= 29, u64 n <= 64, u128 n <= 132,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;           /* highest index computed in cache */
    static const seq_t init[12] = {
        (seq_t)49, (seq_t)87, (seq_t)167, (seq_t)299,
        (seq_t)564, (seq_t)1086, (seq_t)2045, (seq_t)3870,
        (seq_t)7371, (seq_t)14001, (seq_t)26647, (seq_t)50766
    };

    /* Return already computed value if available */
    if (n <= cache_size)
        return cache[n];

    /* Allocate or grow cache to accommodate up to n */
    size_t new_cap = (size_t)n + 1;           /* indices 0..n */
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) {
        /* Allocation failed – return 0 (not expected in tests) */
        return (seq_t)0;
    }
    cache = tmp;

    /* Compute terms from cache_size+1 up to n */
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        seq_t val;
        if (i <= 12) {
            val = init[(size_t)i - 1];
        } else {
            /* Recurrence: a(n) = a(n-1)+a(n-2)+a(n-3)+2*a(n-4)-2*a(n-5)+2*a(n-6)-4*a(n-7)-3*a(n-8)+2*a(n-9)-4*a(n-10)+2*a(n-11) */
            val = cache[i-1];
            val += cache[i-2];
            val += cache[i-3];
            val += (seq_t)2 * cache[i-4];
            val -= (seq_t)2 * cache[i-5];
            val += (seq_t)2 * cache[i-6];
            val -= (seq_t)4 * cache[i-7];
            val -= (seq_t)3 * cache[i-8];
            val += (seq_t)2 * cache[i-9];
            val -= (seq_t)4 * cache[i-10];
            val += (seq_t)2 * cache[i-11];
        }
        cache[i] = val;
    }

    cache_size = n;
    return cache[n];
}
