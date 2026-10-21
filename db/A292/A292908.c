/*
 * A292908  Expansion of e.g.f. exp(x^3 * exp(-x)).
 * https://oeis.org/A292908
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = n! * Sum_{k=0..floor(n/3)} (-k)^(n-3*k)/(k! * (n-3*k)!)
 * Range:     i8 n <= 5, i16 n <= 7, i32 n <= 12, i64 n <= 21, i128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cached = 0;  /* number of values already computed */

seq_t a(idx_t n) {
    /* Grow and compute values up to n if needed */
    if (cached <= n) {
        /* Determine new capacity */
        idx_t new_cap = (cached ? cached * 2 : 16);
        if (new_cap <= n + 1)
            new_cap = n + 1;

        cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));

        /* Compute missing values */
        for (idx_t i = cached; i <= n; ++i) {
            seq_t val;
            if (i == 0)
                val = 1;
            else if (i == 1 || i == 2)
                val = 0;
            else {
                seq_t binom = 1;      /* binom(n-1,0) */
                seq_t sum   = 0;

                for (idx_t m = 1; m < i; ++m) {
                    /* Update binom to binom(i-1,m) */
                    binom = binom * (i - m) / m;

                    if (m >= 2) {
                        seq_t term = binom;
                        term *= (m + 1);   /* (m+1) */
                        term *= m;         /* m */
                        term *= (m - 1);   /* (m-1) */
                        term *= cache[i - m - 1];

                        if (m & 1)          /* (-1)^m */
                            term = -term;

                        sum += term;
                    }
                }
                val = sum;
            }
            cache[i] = val;
        }
        cached = n + 1;
    }
    return cache[n];
}
