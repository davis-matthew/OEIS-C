/*
 * A379885  E.g.f. A(x) satisfies A(x) = 1/(exp(-x) - x*A(x)).
 * https://oeis.org/A379885
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS: A379885
   Formula: a(n) = n! * Sum_{k=0..n} (2*n-2*k+1)^(k-1) * binomial(2*n-2*k+1,n-k)/k!
   Recurrence: a(0)=1
   For n>=1:
     a(n) = n * Sum_{i=0}^{n-1} binom(n-1,i) * a(i) * a(n-1-i)
            - Sum_{k=0}^{n-1} binom(n,k) * a(k) * (-1)^{n-k}
   This implementation uses the recurrence; values are memoised.
   Supports arbitrary-precision seq_t (unsigned or wide integers). */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache of computed values */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* number of entries currently valid */
    static size_t cache_cap = 0;  /* allocated capacity */

    /* allocate or grow cache */
    if (n + 1 > cache_cap) {
        size_t new_cap = n + 1;
        seq_t *new_mem = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_mem) abort();  /* memory allocation failed */
        cache = new_mem;
        cache_cap = new_cap;
    }

    /* initialise first term */
    if (cache_len == 0) {
        cache[0] = 1;
        cache_len = 1;
    }

    /* compute values up to n if necessary */
    for (idx_t m = cache_len; m <= n; ++m) {
        /* part 1: m * Sum_{i=0}^{m-1} binom(m-1,i) * a(i) * a(m-1-i) */
        seq_t sum1 = 0;
        seq_t bin2 = 1;  /* binom(m-1,0) */
        for (idx_t i = 0; i < m; ++i) {
            seq_t prod = bin2 * cache[i] * cache[m - 1 - i];
            sum1 += prod;
            /* update bin2 to binom(m-1,i+1) */
            bin2 = (bin2 * (seq_t)(m - 1 - i)) / (seq_t)(i + 1);
        }
        seq_t temp = (seq_t)m * sum1;  /* m * sum1 */

        /* part 2: subtract/ add terms from second sum */
        seq_t bin = 1;  /* binom(m,0) */
        for (idx_t k = 0; k < m; ++k) {
            seq_t term = bin * cache[k];
            if (((m - k) & 1) == 0)  /* even: positive sign => subtract */
                temp -= term;
            else                    /* odd : negative sign => add */
                temp += term;
            /* update bin to binom(m,k+1) */
            bin = (bin * (seq_t)(m - k)) / (seq_t)(k + 1);
        }

        cache[m] = temp;
    }

    cache_len = n + 1;
    return cache[n];
}
