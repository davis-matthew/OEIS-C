/*
 * A301962  Number of nX6 0..1 arrays with every element equal to 0, 1 or 4
 *          horizontally or antidiagonally adjacent elements, with upper left
 *          element zero.
 * https://oeis.org/A301962
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) +8*a(n-2) +4*a(n-3) -8*a(n-4) -7*a(n-5) -a(n-6) *
 *            -3*a(n-7) +a(n-8) +5*a(n-9) -3*a(n-10) +a(n-11) for n>12 *
 *            Initial values: * a(1)=13, a(2)=22, a(3)=74, a(4)=219, a(5)=749,
 *            a(6)=2544, * a(7)=8705, a(8)=29750, a(9)=101869, a(10)=348726,
 *            a(11)=1194018, * a(12)=4088095
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 35, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* highest index already computed */
    static idx_t cache_cap  = 0;   /* allocated capacity (>= cache_size+1) */

    /* initialise cache on first call */
    if (cache_cap == 0) {
        cache_cap = 13;  /* indices 0 .. 12 */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;   /* allocation failed */

        cache[0]  = 0;
        cache[1]  = (seq_t)13;
        cache[2]  = (seq_t)22;
        cache[3]  = (seq_t)74;
        cache[4]  = (seq_t)219;
        cache[5]  = (seq_t)749;
        cache[6]  = (seq_t)2544;
        cache[7]  = (seq_t)8705;
        cache[8]  = (seq_t)29750;
        cache[9]  = (seq_t)101869;
        cache[10] = (seq_t)348726;
        cache[11] = (seq_t)1194018;
        cache[12] = (seq_t)4088095;
        cache_size = 12;
    }

    /* return if already known */
    if (n <= cache_size) return cache[n];

    /* extend cache up to n */
    for (idx_t k = cache_size + 1; k <= n; ++k) {
        /* compute a(k) using the recurrence */
        seq_t pos = 0;
        pos += cache[k-1] * (seq_t)1;
        pos += cache[k-2] * (seq_t)8;
        pos += cache[k-3] * (seq_t)4;
        pos += cache[k-8] * (seq_t)1;
        pos += cache[k-9] * (seq_t)5;
        pos += cache[k-11] * (seq_t)1;

        seq_t neg = 0;
        neg += cache[k-4] * (seq_t)8;
        neg += cache[k-5] * (seq_t)7;
        neg += cache[k-6] * (seq_t)1;
        neg += cache[k-7] * (seq_t)3;
        neg += cache[k-10] * (seq_t)3;

        seq_t val = pos - neg;

        /* grow capacity if needed (doubling strategy) */
        if (k >= cache_cap) {
            idx_t new_cap = cache_cap;
            while (new_cap <= k) new_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) {
                free(cache);
                cache = NULL;
                cache_cap = 0;
                cache_size = 0;
                return (seq_t)0;   /* allocation error */
            }
            cache = tmp;
            cache_cap = new_cap;
        }

        cache[k] = val;
        cache_size = k;
    }

    return cache[n];
}
