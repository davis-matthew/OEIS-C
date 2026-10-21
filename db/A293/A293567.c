/*
 * A293567  Expansion of e.g.f.: exp(x^2/(x^3 - 1)).
 * https://oeis.org/A293567
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) satisfies the D-finite recurrence
 *            n*(n+1)*(n+2)*(n+3)*(n+4)*(n+5)*a(n) +
 *            (n+5)*(n+4)*(n+3)*(n+2)*a(n+1) - 2*(n+5)*(n+4)*(n+3)*a(n+3) +
 *            2*(n+5)*a(n+4) + a(n+6) = 0 Initial values (offset 0): a(0) = 1,
 *            a(1) = 0, a(2) = -2, a(3) = 0, a(4) = 12, a(5) = -120
 * Range:     i8 n <= 6, i16 n <= 7, i32 n <= 12, i64 n <= 20, i128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;     /* sequence values */
    static size_t capacity = 0;     /* allocated capacity */
    static size_t size = 0;         /* number of values stored */

    /* initialise when called for the first time */
    if (cache == NULL) {
        capacity = 16;
        cache = (seq_t*)malloc(capacity * sizeof(seq_t));
        if (!cache) abort();

        /* first six terms */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)-2;
        cache[3] = (seq_t)0;
        cache[4] = (seq_t)12;
        cache[5] = (seq_t)-120;
        /* zero the unused slots */
        for (size_t i = 6; i < capacity; ++i) cache[i] = (seq_t)0;
        size = 6;
    }

    /* if n already computed return it */
    if (n < size)
        return cache[n];

    /* grow the cache if necessary */
    if (n >= capacity) {
        size_t new_cap = capacity;
        while (new_cap <= (size_t)n) {
            new_cap = (new_cap < 16) ? 16 : new_cap << 1;
        }
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        for (size_t i = capacity; i < new_cap; ++i) cache[i] = (seq_t)0;
        capacity = new_cap;
    }

    /* compute remaining terms up to n using the recurrence */
    for (idx_t i = size; i <= n; ++i) {
        /* i corresponds to a(i) = a(n+6) with n = i-6 */

        /* term1: (i-6)*(i-5)*(i-4)*(i-3)*(i-2)*(i-1)*a[i-6] */
        seq_t t1 = cache[i-6];
        t1 *= (seq_t)(i-6);
        t1 *= (seq_t)(i-5);
        t1 *= (seq_t)(i-4);
        t1 *= (seq_t)(i-3);
        t1 *= (seq_t)(i-2);
        t1 *= (seq_t)(i-1);

        /* term2: (i-1)*(i-2)*(i-3)*(i-4)*a[i-5] */
        seq_t t2 = cache[i-5];
        t2 *= (seq_t)(i-1);
        t2 *= (seq_t)(i-2);
        t2 *= (seq_t)(i-3);
        t2 *= (seq_t)(i-4);

        /* term3: 2*(i-1)*(i-2)*(i-3)*a[i-3]  (will be subtracted) */
        seq_t t3 = cache[i-3];
        t3 *= (seq_t)(i-1);
        t3 *= (seq_t)(i-2);
        t3 *= (seq_t)(i-3);
        t3 *= (seq_t)2;

        /* term4: 2*(i-1)*a[i-2] */
        seq_t t4 = cache[i-2];
        t4 *= (seq_t)(i-1);
        t4 *= (seq_t)2;

        /* S = term1 + term2 - term3 + term4 */
        seq_t S = t1 + t2;
        S -= t3;
        S += t4;

        /* a[i] = -S */
        cache[i] = -S;
    }

    size = (size_t)n + 1;
    return cache[n];
}
