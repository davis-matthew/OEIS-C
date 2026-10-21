/*
 * A249310  Expansion of x*(1+7*x-6*x^3)/(1-8*x^2+6*x^4).
 * https://oeis.org/A249310
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 23, u64 n <= 45, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  
    OEIS A249310: a(n) = coefficient of x^n in
    x*(1+7*x-6*x^3) / (1-8*x^2+6*x^4)

    Recurrence (derived from the denominator):
        a(1) = 1,  a(2) = 7,  a(3) = 8,  a(4) = 50
        for n ≥ 5   a(n) = 8*a(n-2) – 6*a(n-4)
*/

#include <stdlib.h>

seq_t a(idx_t n)
{
    /*  static buffer for memoisation  */
    static seq_t *cache = NULL;   /* 1‑based index: cache[0] unused  */
    static idx_t  last   = 0;     /* largest n that has been computed */
    static idx_t  cap    = 0;     /* allocated capacity (>= last+1)   */

    if (n <= last)        /* already known */
        return cache[n];

    /*  enlarge the buffer if required  */
    if (n > cap) {
        idx_t new_cap = n * 2;          /* growth factor 2  */
        if (new_cap < n) new_cap = n;  /* guard against overflow */

        seq_t *tmp = (seq_t*)realloc(cache, (new_cap + 1) * sizeof(seq_t));
        if (!tmp) abort();             /* allocation failed */
        cache = tmp;
        cap   = new_cap;
    }

    /*  compute new terms in increasing order  */
    for (idx_t i = last + 1; i <= n; ++i) {
        if (i == 1)
            cache[i] = 1;
        else if (i == 2)
            cache[i] = 7;
        else if (i == 3)
            cache[i] = 8;
        else if (i == 4)
            cache[i] = 50;
        else
            cache[i] = cache[i-2] * (seq_t)8 - cache[i-4] * (seq_t)6;
    }
    last = n;

    return cache[n];
}
