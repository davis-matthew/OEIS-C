/*
 * A347153  Sum of all divisors, except the largest of every number, of the
 *          first n odd numbers.
 * https://oeis.org/A347153
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=1..n} (sigma(2k-1) - (2k-1))
 * Range:     u8 n <= 24, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* number of terms stored so far */
    static size_t cache_cap = 0;   /* allocation capacity of cache */

    if (n == 0) return (seq_t)0;           /* safety; not used by OEIS */

    /* extend cache if required */
    if (n > cache_size) {
        /* enlarge allocation */
        if (n > cache_cap) {
            size_t new_cap = (cache_cap > 0 ? cache_cap : 16);
            while (new_cap < (size_t)n) new_cap <<= 1;
            cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            cache_cap = new_cap;
        }

        /* compute missing terms */
        for (idx_t i = cache_size; i < n; ++i) {
            idx_t k   = i + 1;                /* 1‑based index */
            idx_t odd = 2 * k - 1;            /* the odd number */

            /* compute sigma(odd) using prime factorisation */
            seq_t sigma = 1;
            idx_t m = odd;
            for (idx_t p = 3; (idx_t)p * (idx_t)p <= m; p += 2) {
                if (m % p == 0) {
                    int exp = 0;
                    while (m % p == 0) {
                        m /= p;
                        ++exp;
                    }
                    /* term = (p^(exp+1)-1)/(p-1) */
                    seq_t power = 1;
                    for (int e = 0; e < exp + 1; ++e) power *= (seq_t)p;
                    seq_t numerator   = power - 1;
                    seq_t denominator = (seq_t)(p - 1);
                    seq_t term        = numerator / denominator;
                    sigma *= term;
                }
            }
            if (m > 1) { /* remaining prime factor */
                seq_t term = (((seq_t)m * (seq_t)m) - 1) / ((seq_t)(m - 1));
                sigma *= term;
            }

            seq_t sumDiv = sigma - (seq_t)odd;       /* proper divisors sum */
            seq_t cum = sumDiv;
            if (i > 0) cum += cache[i - 1];
            cache[i] = cum;
        }
        cache_size = n;
    }
    return cache[n - 1];   /* a(1) is stored at cache[0] */
}
