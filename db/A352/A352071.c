/*
 * A352071  Expansion of e.g.f. 1 / (1 + log(1 - 4*x) / 4).
 * https://oeis.org/A352071
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A352071
 * Recurrence: a(0) = 1
 *             a(n) = Σ_{k=1..n} binom(n,k) * (k-1)! * 4^(k-1) * a(n-k)
 */

#include <stdlib.h>   /* malloc, realloc, abort */

/* Computes a(n) for the OEIS sequence A352071.
 * seq_t and idx_t are defined by the surrounding build. */
seq_t a(idx_t n)
{
    /* Permanent cache of computed values. */
    static seq_t *cache   = NULL;  /* array of a(i) */
    static size_t cap     = 0;     /* allocated size of cache */
    static idx_t  done    = 0;     /* number of values already computed */

    /* Ensure the cache is large enough to hold a(n). */
    if (n >= cap) {
        size_t new_cap = cap ? cap << 1 : 16;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();          /* fatal allocation failure */
        cache = tmp;
        cap   = new_cap;
    }

    /* Compute missing terms up to and including a(n). */
    if (n >= done) {
        for (idx_t m = done; m <= n; ++m) {
            if (m == 0) {            /* a(0) = 1 */
                cache[0] = (seq_t)1;
                continue;
            }

            seq_t sum   = (seq_t)0;
            seq_t binom = (seq_t)1;        /* binom(m,0) = 1 */
            seq_t fact  = (seq_t)1;        /* (k-1)!   for current k */
            seq_t pow4  = (seq_t)1;        /* 4^(k-1)  for current k */

            for (idx_t k = 1; k <= m; ++k) {
                /* Update binomial coefficient C(m,k). */
                binom = binom * (seq_t)(m - k + 1) / (seq_t)k;

                /* (k-1)! * 4^(k-1) */
                seq_t fk = fact * pow4;

                /* Accumulate the term into the sum. */
                sum += binom * fk * cache[m - k];

                /* Prepare for next k. */
                fact *= (seq_t)k;          /* (k)!   for next step */
                pow4  *= (seq_t)4;          /* 4^k    for next step */
            }
            cache[m] = sum;
        }
        done = n + 1;
    }

    return cache[n];
}
