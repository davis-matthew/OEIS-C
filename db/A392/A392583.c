/*
 * A392583  Expansion of 1 / ((1-x)^5 - x^2)^2.
 * https://oeis.org/A392583
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 17, u64 n <= 37, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 10*a(n-1) - 43*a(n-2) + 110*a(n-3) - 191*a(n-4)
 *               + 232*a(n-5) - 200*a(n-6) + 118*a(n-7) - 45*a(n-8)
 *               + 10*a(n-9) - a(n-10)
 * Initial values: a(0)=1, a(1)=10, a(2)=57, a(3)=250, a(4)=958, a(5)=3422,
 *                a(6)=11759, a(7)=39416, a(8)=129700, a(9)=420542 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;          /* highest index that has been computed, inclusive */
    static size_t cache_cap = 0;         /* allocated capacity in elements */

    /* constants for the first 10 terms */
    static const seq_t init[10] = {
        (seq_t)1,     (seq_t)10,   (seq_t)57,   (seq_t)250,
        (seq_t)958,   (seq_t)3422, (seq_t)11759, (seq_t)39416,
        (seq_t)129700, (seq_t)420542
    };

    /* Ensure allocated space for indices 0..n (at least 10 for the defaults) */
    size_t needed = (size_t)(n + 1);
    if (needed < 10) needed = 10;
    if (cache_cap < needed) {
        seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (!tmp) {        /* allocation failed – return 0 (unlikely for the given limits) */
            return (seq_t)0;
        }
        cache = tmp;
        cache_cap = needed;
    }

    /* Initialize the first 10 values once */
    if (cache_len < 9) {
        for (idx_t i = 0; i <= 9; ++i)
            cache[i] = init[i];
        cache_len = 9;
    }

    /* Compute any missing terms up to n using the recurrence */
    if (cache_len < n) {
        for (idx_t i = cache_len + 1; i <= n; ++i) {
            seq_t pos = 0, neg = 0;

            /* Positive coefficient terms */
            pos += (seq_t)10  * cache[i-1];
            pos += (seq_t)110 * cache[i-3];
            pos += (seq_t)232 * cache[i-5];
            pos += (seq_t)118 * cache[i-7];
            pos += (seq_t)10  * cache[i-9];

            /* Negative coefficient terms */
            neg += (seq_t)43  * cache[i-2];
            neg += (seq_t)191 * cache[i-4];
            neg += (seq_t)200 * cache[i-6];
            neg += (seq_t)45  * cache[i-8];
            neg += (seq_t)1   * cache[i-10];

            /* Final value: pos - neg (guaranteed non-negative) */
            cache[i] = pos - neg;
        }
        cache_len = n;
    }

    return cache[n];
}
