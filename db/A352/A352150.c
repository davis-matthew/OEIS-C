/*
 * A352150  a(n) = Sum_{k=0..n-1} (-1)^k * binomial(n,k)^2 * (n-k-1)!.
 * https://oeis.org/A352150
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=0..n-1} (-1)^k * binomial(n,k)^2 * (n-k-1)!
 * Range:     i8 n <= 6, i16 n <= 9, i32 n <= 14, i64 n <= 21, i128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Compute factorials up to n-1 in a static cache. */
static seq_t *fact_cache = NULL;
static idx_t fact_cache_size = 0;

/* Return a(n). */
seq_t a(idx_t n)
{
    /* a(0) = 0 by definition. */
    if (n == 0)
        return 0;

    /* Size needed for factorials: indices 0 .. n-1  (n elements). */
    idx_t needed = n;

    /* Reserve or grow the factorial cache. */
    if (fact_cache == NULL) {
        fact_cache_size = needed;
        fact_cache = (seq_t *)malloc(fact_cache_size * sizeof(seq_t));
        if (!fact_cache)
            exit(EXIT_FAILURE);
        fact_cache[0] = 1;                     /* 0! */
        for (idx_t i = 1; i < fact_cache_size; ++i)
            fact_cache[i] = fact_cache[i-1] * (seq_t)i;
    } else if (needed > fact_cache_size) {
        seq_t *tmp = (seq_t *)realloc(fact_cache,
                                      needed * sizeof(seq_t));
        if (!tmp)
            exit(EXIT_FAILURE);
        for (idx_t i = fact_cache_size; i < needed; ++i)
            tmp[i] = tmp[i-1] * (seq_t)i;
        fact_cache = tmp;
        fact_cache_size = needed;
    }

    seq_t sum = 0;
    bool sign = true;   /* (-1)^0 = +1 */
    seq_t binom = 1;    /* binomial(n,0) */

    for (idx_t k = 0; k < n; ++k) {
        /* term = binom^2 * (n-k-1)! */
        seq_t term = binom * binom;
        term *= fact_cache[n - k - 1];
        if (!sign)
            term = -term;
        sum += term;
        sign = !sign;

        /* Update binom to binomial(n, k+1) for next iteration. */
        if (k + 1 < n)
            binom = binom * (seq_t)(n - k) / (seq_t)(k + 1);
    }
    return sum;
}
