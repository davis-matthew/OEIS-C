/*
 * A375532  a(n) is the denominator of Sum_{k = 1..n} k! / A375531(k).
 * https://oeis.org/A375532
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n+1) = (n+1)! * a(n)^2 + a(n), with a(0)=1, a(1)=2
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap = 0;   /* allocated capacity */
    static size_t len = 0;   /* number of terms stored (highest index + 1) */
    static seq_t fact = (seq_t)1;  /* factorial of len (i.e., factorial of len) */

    /* Initial allocation */
    if (cap == 0) {
        cap = 2;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        cache[0] = (seq_t)1;   /* a(0) */
        len = 1;
        fact = (seq_t)1;       /* 1! = 1 */
    }

    /* Return already computed value */
    if (n < len)
        return cache[n];

    /* Ensure a(1) is stored if needed */
    if (len < 2) {
        if (cap < 2) {
            cap = 2;
            cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
        }
        cache[1] = (seq_t)2;   /* a(1) */
        len = 2;
    }

    /* Grow the cache to hold n+1 terms */
    if (n + 1 > cap) {
        size_t newcap = cap;
        while (n + 1 > newcap)
            newcap = (newcap == 0) ? 8 : newcap * 2;
        cache = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        cap = newcap;
    }

    /* Compute missing terms up to n */
    for (idx_t idx = len; idx <= n; ++idx) {
        /* Update factorial to (idx)! */
        fact = fact * (seq_t)idx;

        /* Recurrence: a(idx) = idx! * a(idx-1)^2 + a(idx-1) */
        seq_t prev = cache[idx - 1];
        seq_t temp = prev * prev;   /* a(idx-1)^2 */
        temp = temp * fact;         /* idx! * a(idx-1)^2 */
        temp = temp + prev;         /* plus a(idx-1) */

        cache[idx] = temp;
    }
    len = n + 1;   /* all terms up to n are now stored */

    return cache[n];
}
