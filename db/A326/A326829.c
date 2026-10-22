/*
 * A326829  G.f. A(x) satisfies: A(x) = 1 - x * (1 - A(x/(1 + x)) / (1 + x)).
 * https://oeis.org/A326829
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0) = 1 * a(1) = 0 * a(n) = Sum_{k=1..n} (-1)^(k+1) *
 *            binomial(n-1,k-1) * a(n-k) (n >= 2) * This is a linear
 *            recurrence involving binomial coefficients
 * Range:     i8 n <= 9, i16 n <= 12, i32 n <= 19, i64 n <= 27, i128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Return the n-th term of sequence A326829.
 * The function memoises previously computed values so that calls in any
 * order are efficient.  */
seq_t a(idx_t n)
{
    /* static snapshot of the cache and its current size */
    static seq_t *cache   = NULL;
    static idx_t cache_n  = 0;      /* number of cached elements */

    /* Extend the cache if needed */
    if (n >= cache_n) {
        idx_t new_n    = n + 1;                  /* new cache size */
        seq_t *new_buf = (seq_t *)malloc(new_n * sizeof(seq_t));

        /* Copy old values (if any) to the new buffer */
        for (idx_t i = 0; i < cache_n; ++i)
            new_buf[i] = cache[i];

        /* Initialise the first two terms */
        if (cache_n == 0) {
            new_buf[0] = 1;
            if (new_n > 1) new_buf[1] = 0;
        }

        /* Compute terms that were not yet cached */
        for (idx_t i = cache_n; i < new_n; ++i) {
            if (i == 0) { new_buf[i] = 1; continue; }
            if (i == 1) { new_buf[i] = 0; continue; }

            seq_t sum   = 0;
            seq_t binom = 1;                      /* C(i-1,0) */

            for (idx_t j = 0; j <= i-1; ++j) {
                if (j > 0) {
                    /* update binom to C(i-1,j) iteratively
                     *  binom = binom * (i - j) / j         */
                    binom = binom * (seq_t)(i - j) / (seq_t)j;
                }

                /* Sign factor (-1)^(i-j+1) */
                bool odd_sig = (((i - j + 1) & 1) != 0);

                seq_t term = new_buf[j] * binom;
                if (odd_sig) term = -term;
                sum += term;
            }
            new_buf[i] = sum;
        }

        /* Replace old cache */
        free(cache);
        cache   = new_buf;
        cache_n = new_n;
    }

    return cache[n];
}
