/*
 * A299576  Number of nX3 0..1 arrays with every element equal to 0, 3, 4, 5,
 *          7 or 8 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A299576
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1)+a(n-2)+a(n-3)-4*a(n-4)-2*a(n-5)-2*a(n-6)+a(n-9)
 *            for n>13, with initial values a(1)=1, a(2)=1, a(3)=10, a(4)=10,
 *            a(5)=27, a(6)=63, a(7)=115, a(8)=267, a(9)=569, a(10)=1193,
 *            a(11)=2587, a(12)=5521, a(13)=11791
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 29, u64 n <= 58, u128 n <= 117,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Header comments above match the required function signature. */
seq_t a(idx_t n)
{
    /* Sequence values for 1..13 */
    static const seq_t base[14] = {
        0,
        1, 1, 10, 10, 27, 63, 115, 267, 569,
        1193, 2587, 5521, 11791
    };

    /* Memoization cache */
    static seq_t *cache = NULL;
    static size_t cap = 0;      /* allocated size */
    static idx_t max_cached = 0;/* largest n computed (>=1) */

    if (n < 1)
        return 0;   /* undefined for n<1; return 0 for safety */

    /* Ensure cache can hold up to n */
    if (cache == NULL) {
        cap = (size_t)n + 1;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (cache == NULL)
            return 0;   /* allocation failure */
    } else if (cap < (size_t)n + 1) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL)
            return 0;   /* allocation failure */
        cache = tmp;
        cap = new_cap;
    }

    /* Fill base values up to 13 if not already present */
    if (max_cached < 13) {
        idx_t last = (max_cached >= 13) ? 13 : max_cached;
        for (idx_t i = max_cached + 1; i <= 13 && i <= n; ++i)
            cache[i] = base[i];
        if (last < 13)
            max_cached = last;
    }

    /* Compute values beyond cached ones */
    for (idx_t i = max_cached + 1; i <= n; ++i) {
        if (i <= 13) {
            cache[i] = base[i];
        } else {
            seq_t sum = cache[i-1] * (seq_t)2 + cache[i-2] + cache[i-3] + cache[i-9];
            seq_t sub = cache[i-4] * (seq_t)4 + cache[i-5] * (seq_t)2 + cache[i-6] * (seq_t)2;
            cache[i] = sum - sub;
        }
    }
    max_cached = n;
    return cache[n];
}
