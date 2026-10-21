/*
 * A259861  a(n+8)+34*a(n+4)+a(n)=0 with a(0)-a(7) as shown.
 * https://oeis.org/A259861
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n+8)+34*a(n+4)+a(n)=0 (recurrence: a(n)=-(34*a(n-4)+a(n-8))
 *            for n>=8) * Initial values: * a(0)= 2, * a(1)= 2, * a(2)=- 5, *
 *            a(3)=-29, * a(4)=-70, * a(5)=-70, * a(6)=169, * a(7)=985
 * Range:     i8 n <= 5, i16 n <= 10, i32 n <= 23, i64 n <= 49, i128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Static cache to store already computed values.
     * cache_cap is the allocated size, cache_size is the highest
     * index that has been computed + 1.
     */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    static idx_t cache_cap = 0;

    /* Ensure capacity for index n */
    if (n >= cache_cap) {
        idx_t new_cap = n + 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* If value already computed, return it */
    if (n < cache_size) {
        return cache[n];
    }

    /* Initialize base values if not already done */
    static const seq_t init[8] = { (seq_t)2,  (seq_t)2,  (seq_t)-5, (seq_t)-29,
                                   (seq_t)-70, (seq_t)-70, (seq_t)169, (seq_t)985 };

    /* Compute missing values up to n */
    for (idx_t i = cache_size; i <= n; ++i) {
        if (i < 8) {
            cache[i] = init[i];
        } else {
            /* a(i) = -(34*a(i-4) + a(i-8)) */
            seq_t term = 34 * cache[i - 4] + cache[i - 8];
            cache[i] = -term;
        }
    }

    cache_size = n + 1;
    return cache[n];
}
