/*
 * A252253  Number of (n+2) X (7+2) 0..3 arrays with every 3 X 3 subblock row
 *          and column sum not equal to 0 3 5 6 or 7 and every 3 X 3 diagonal
 *          and antidiagonal sum equal to 0 3 5 6 or 7.
 * https://oeis.org/A252253
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 3*a(n-3) - 3*a(n-4) - a(n-6) + a(n-7) Initial
 *            values: a(1)=847, a(2)=437, a(3)=366, a(4)=384, a(5)=414,
 *            a(6)=453, a(7)=501, a(8)=579, a(9)=681
 * Range:     u8 none, u16 n <= 25, u32 n <= 59, u64 n <= 128, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache to store computed values; indexed by n (1‑based) */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;

    if (n == 0) return 0;          /* offset is 1 */

    size_t need = (size_t)n + 1;   /* array length needed (index 0 unused) */

    /* Allocate or grow the cache to at least 'need' elements */
    if (cache == NULL) {
        cache_cap = need > 10 ? need : 10;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    } else if (cache_cap < need) {
        cache = (seq_t *)realloc(cache, need * sizeof(seq_t));
        cache_cap = need;
    }

    /* Initial values for indices 1..9 */
    static const seq_t init_vals[10] = {0, 847, 437, 366, 384, 414,
                                       453, 501, 579, 681};

    /* Compute values from cache_len+1 up to n */
    for (size_t i = cache_len + 1; i <= n; ++i) {
        if (i <= 9) {
            cache[i] = init_vals[(size_t)i];
        } else {
            /* Use the recurrence for i > 9 */
            seq_t t1 = cache[i - 1];
            seq_t t2 = cache[i - 3] * 3;
            seq_t t3 = cache[i - 7];
            seq_t sum = t1 + t2 + t3;
            sum = sum - cache[i - 4] * 3;
            sum = sum - cache[i - 6];
            cache[i] = sum;
        }
    }

    cache_len = n;
    return cache[n];
}
