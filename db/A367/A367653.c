/*
 * A367653  G.f. A(x) satisfies A(x) = 1 / (1 - x * (1 + x + x^2 + x^3) *
 *          A(x^4)).
 * https://oeis.org/A367653
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0}^{n-1} a(floor(k/4)) * a(n-1-k), a(0)=1
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* Computes OEIS A367653: a(n) = Σ_{k=0}^{n-1} a(⌊k/4⌋) * a(n-1-k), a(0)=1. */
seq_t a(idx_t n)
{
    /* static cache of computed values */
    static seq_t *aval   = NULL;   /* a[0 .. len-1]   */
    static seq_t *pref   = NULL;   /* pref[0 .. len-1] = Σ_{t=0}^{i} a[t] */
    static size_t cap   = 0;       /* allocated capacity */
    static size_t len   = 0;       /* number of elements computed */

    /* grow on demand */
    if ((size_t)n + 1 > cap) {
        size_t newcap = cap ? cap * 2 : 8;
        while (newcap < (size_t)n + 1)
            newcap *= 2;
        aval = (seq_t *)realloc(aval, newcap * sizeof(seq_t));
        pref = (seq_t *)realloc(pref, newcap * sizeof(seq_t));
        cap = newcap;
    }

    /* compute missing values up to n */
    for (size_t i = len; i <= (size_t)n; ++i) {
        seq_t val = 0;
        if (i == 0) {
            val = (seq_t)1;
        } else {
            idx_t limit = (idx_t)(i - 1);          /* last k index in the sum */
            for (idx_t m = 0; 4 * m <= limit; ++m) {
                idx_t k_start = 4 * m;
                idx_t k_end   = k_start + 3;
                if (k_end > limit)
                    k_end = limit;

                /* indices of a(n-1-k) for k in [k_start, k_end] */
                idx_t j_start = limit - k_end;     /* smallest index of a() in sum */
                idx_t j_end   = limit - k_start;   /* largest index of a() in sum */

                seq_t sum_a = pref[j_end];
                if (j_start > 0)
                    sum_a -= pref[j_start - 1];

                val += aval[(size_t)m] * sum_a;
            }
        }
        aval[i] = val;
        if (i == 0)
            pref[i] = val;
        else
            pref[i] = pref[i - 1] + val;
    }
    len = (size_t)n + 1;
    return aval[(size_t)n];
}
