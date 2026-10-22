/*
 * A373662  a(n) = (1 + (n+1)^2 - (n-2)*(-1)^n)/2.
 * https://oeis.org/A373662
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1 + (n+1)^2 - (n-2)*(-1)^n)/2 Recurrence (valid for n ≥
 *            6): a(n) = a(n-1) + 2*a(n-2) - 2*a(n-3) - a(n-4) + a(n-5)
 *            Initial values: a(1)=2, a(2)=5, a(3)=9, a(4)=12, a(5)=20
 * Range:     u8 n <= 22, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Initial five terms (offset 1) */
    static const seq_t init[5] = {2, 5, 9, 12, 20};

    if (n <= 5 && n >= 1)
        return init[n - 1];

    /* Static cache to store computed values */
    static seq_t *cache   = NULL;
    static idx_t  sz      = 0; /* allocated capacity */
    static idx_t  len     = 0; /* number of terms currently computed */

    /* Ensure cache exists and contains at least the first five terms */
    if (len == 0) {
        sz = 8; /* initial capacity */
        cache = (seq_t *)malloc(sz * sizeof(seq_t));
        if (!cache) return 0; /* allocation failure: return 0 as fallback */

        cache[0] = init[0];
        cache[1] = init[1];
        cache[2] = init[2];
        cache[3] = init[3];
        cache[4] = init[4];
        len = 5;
    }

    /* Grow cache if necessary */
    if (n > sz) {
        idx_t new_sz = sz;
        while (new_sz < n) new_sz <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_sz * sizeof(seq_t));
        if (!tmp) return 0; /* allocation failure: return 0 as fallback */
        cache = tmp;
        sz = new_sz;
    }

    /* Compute missing terms up to n */
    for (idx_t k = len + 1; k <= n; ++k) {
        /* Compute a(k) using the recurrence with unsigned arithmetic.
           sum1 = a(k-1) + 2*a(k-2) + a(k-5)
           sum2 = a(k-4) + 2*a(k-3)
           a(k) = sum1 - sum2
        */
        seq_t sum1 = cache[k - 2];          /* a(k-1) */
        sum1 += cache[k - 3];               /* + a(k-2) */
        sum1 += cache[k - 3];               /* + a(k-2) again (×2) */
        sum1 += cache[k - 6];               /* + a(k-5) */

        seq_t sum2 = cache[k - 5];          /* a(k-4) */
        sum2 += cache[k - 4];               /* + a(k-3) */
        sum2 += cache[k - 4];               /* + a(k-3) again (×2) */

        cache[k - 1] = sum1 - sum2;         /* a(k) */
    }

    len = n;
    return cache[n - 1];
}
