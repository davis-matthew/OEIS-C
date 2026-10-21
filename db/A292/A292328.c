/*
 * A292328  p-INVERT of the Fibonacci sequence (A000045), where p(S) = (1 -
 *          S)^3.
 * https://oeis.org/A292328
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) - 9*a(n-2) - 4*a(n-3) + 9*a(n-4) + 6*a(n-5) +
 *            a(n-6) with initial values a(0)=3, a(1)=9, a(2)=28, a(3)=84,
 *            a(4)=246, a(5)=707, a(6)=2001
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 20, u64 n <= 44, u128 n <= 93,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cacheSize = 0;
    static const seq_t init[7] = {
        (seq_t)3, (seq_t)9, (seq_t)28, (seq_t)84,
        (seq_t)246, (seq_t)707, (seq_t)2001
    };

    /* Ensure cache large enough for requested n */
    if (cacheSize == 0) {
        cacheSize = (n + 1 > 7) ? (n + 1) : 7;
        cache = (seq_t *)malloc(sizeof(seq_t) * cacheSize);
        for (idx_t i = 0; i < 7; ++i) cache[i] = init[i];
        for (idx_t i = 7; i < cacheSize; ++i) {
            seq_t a1 = cache[i - 1];
            seq_t a2 = cache[i - 2];
            seq_t a3 = cache[i - 3];
            seq_t a4 = cache[i - 4];
            seq_t a5 = cache[i - 5];
            seq_t a6 = cache[i - 6];
            cache[i] = 6 * a1 - 9 * a2 - 4 * a3 + 9 * a4 + 6 * a5 + a6;
        }
    } else if (n + 1 > cacheSize) {
        idx_t newSize = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * newSize);
        if (!tmp) return init[0];  /* allocation failure, unlikely */
        cache = tmp;
        for (idx_t i = cacheSize; i <= n; ++i) {
            seq_t a1 = cache[i - 1];
            seq_t a2 = cache[i - 2];
            seq_t a3 = cache[i - 3];
            seq_t a4 = cache[i - 4];
            seq_t a5 = cache[i - 5];
            seq_t a6 = cache[i - 6];
            cache[i] = 6 * a1 - 9 * a2 - 4 * a3 + 9 * a4 + 6 * a5 + a6;
        }
        cacheSize = newSize;
    }
    return cache[n];
}
