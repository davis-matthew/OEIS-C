/*
 * A291983  Expansion of 1/((1+x)*(1+x^2)*(1+x^3)).
 * https://oeis.org/A291983
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = coefficient of x^n in 1/((1+x)(1+x^2)(1+x^3)) Recurrence:
 *            a(0)=1; for n≥1 a(n) =
 *            -(a(n‑1)+a(n‑2)+2·a(n‑3)+a(n‑4)+a(n‑5)+a(n‑6)) (a(k)=0 for k<0)
 * Range:     i8 n <= 70, i16 n <= 70, i32 n <= 70, i64 n <= 70, i128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache of computed values; index 0..cached-1 are valid */
    static seq_t *cache = NULL;
    static size_t cached = 0;           /* number of entries already stored */

    if ((size_t)n >= cached) {
        size_t need = (size_t)n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, need * sizeof(seq_t));
        if (!tmp) return 0;             /* allocation failure – return 0 */
        cache = tmp;
        if (cached == 0)               /* first time: set a(0)=1 */
            cache[0] = 1;
        /* Compute missing entries from the end of the existing cache */
        for (size_t i = (cached == 0 ? 1 : cached); i <= (size_t)n; ++i) {
            seq_t sum = 0;
            if (i >= 1) sum += cache[i-1];
            if (i >= 2) sum += cache[i-2];
            if (i >= 3) { sum += cache[i-3]; sum += cache[i-3]; } /* 2*a(i-3) */
            if (i >= 4) sum += cache[i-4];
            if (i >= 5) sum += cache[i-5];
            if (i >= 6) sum += cache[i-6];
            cache[i] = -sum;
        }
        cached = n + 1;
    }
    return cache[(size_t)n];
}
