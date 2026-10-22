/*
 * A320433  Expansion of e.g.f. exp(4 * (1 - exp(x)) + x).
 * https://oeis.org/A320433
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0) = 1; a(n) = a(n-1) - 4 * Sum_{k=0}^{n-1} binomial(n-1,k) *
 *            a(k)
 * Range:     i8 n <= 5, i16 n <= 8, i32 n <= 14, i64 n <= 24, i128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static state for memoisation */
    static idx_t  max_n      = 0;      /* largest n computed so far   */
    static size_t capacity   = 0;      /* current allocation capacity */
    static seq_t *seq        = NULL;   /* a[0 .. capacity-1]          */
    static seq_t *binom      = NULL;   /* binomial row for current n  */

    /* first call: reserve space for n = 0 and initialise a(0) and binom(0) */
    if (capacity == 0) {
        capacity = 1;
        seq  = (seq_t*)realloc(NULL, capacity * sizeof(seq_t));
        binom= (seq_t*)realloc(NULL, capacity * sizeof(seq_t));
        seq[0]   = (seq_t)1;   /* a(0) = 1 */
        binom[0] = (seq_t)1;   /* binomial(0,0) = 1 */
        max_n = 0;
    }

    /* enlarge the arrays if we need a value beyond the current limit */
    if (n > max_n) {
        size_t new_cap = capacity;
        while (new_cap <= n)              /* round up to a power‑of‑two‑ish size */
            new_cap = new_cap * 2 + 1;

        seq   = (seq_t*)realloc(seq,   new_cap * sizeof(seq_t));
        binom = (seq_t*)realloc(binom, new_cap * sizeof(seq_t));
        capacity = new_cap;

        /* compute all missing terms up to n            */
        /* cur is the last already computed index.      */
        for (idx_t cur = max_n; cur < n; ++cur) {
            seq_t sum = (seq_t)0;
            for (idx_t k = 0; k <= cur; ++k)          /* Sum_{k=0}^{cur} binom(k)*a(k) */
                sum += binom[k] * seq[k];

            seq[cur + 1] = seq[cur] - (seq_t)4 * sum; /* recurrence for a(cur+1) */

            /* update binomial row in place:   binom_{k+1} += binom_k  (reverse order) */
            binom[cur + 1] = (seq_t)0;
            for (idx_t k = cur + 1; k > 0; --k)
                binom[k] += binom[k - 1];
        }
        max_n = n;
    }

    return seq[n];
}
