/*
 * A318179  Expansion of e.g.f. exp((1 - exp(-4*x))/4).
 * https://oeis.org/A318179
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=1..n} (-4)^(k-1)*C(n-1,k-1)*a(n-k) with a(0) = 1.
 *            The function uses lazy memoisation so it works for calls in any
 *            order
 * Range:     i8 n <= 4, i16 n <= 7, i32 n <= 12, i64 n <= 20, i128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* lazy cache of computed values */
    static seq_t *cache = NULL;   /* array of a(k) for k = 0..up_to */
    static size_t cap   = 0;      /* allocated capacity of cache */
    static idx_t  up_to = 0;      /* largest n for which cache[n] is valid */

    /* initialise cache */
    if (cache == NULL) {
        cap   = 1;
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        cache[0] = (seq_t)1;      /* a(0) = 1 */
        up_to   = 0;
    }

    /* if needed, grow cache and compute missing values */
    if (n > up_to) {
        size_t need = n + 1;           /* indices 0..n inclusive */
        if (need > cap) {              /* allocate more space */
            size_t new_cap = cap << 1;
            while (new_cap < need) new_cap <<= 1;
            cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
            cap = new_cap;
        }
        /* compute values from up_to+1 up to n */
        for (idx_t i = up_to + 1; i <= n; ++i) {
            seq_t sum = (seq_t)0;
            seq_t p   = (seq_t)1;     /* (-4)^(k-1) */
            seq_t bin = (seq_t)1;     /* C(i-1, k-1) */

            for (idx_t k = 1; k <= i; ++k) {
                /* term = a(i-k) * bin * p */
                seq_t term = cache[i - k];
                term = term * bin;
                term = term * p;
                sum += term;

                /* update bin and p for next k */
                if (k < i) {
                    bin = bin * (seq_t)(i - k);
                    bin = bin / (seq_t)k;
                }
                p = p * (seq_t)(-4);
            }
            cache[i] = sum;
        }
        up_to = n;
    }

    return cache[n];
}
