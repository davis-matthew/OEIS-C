/*
 * A280556  a(n) = Sum_{k=1..n} k^2 * (k+1)!.
 * https://oeis.org/A280556
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n - 1)*(n + 2)! + 2 (equivalently. a(n) = Σ_{k=1..n}
 *            k²·(k+1)! )
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Return the nth term of OEIS sequence A280556. */
seq_t a(idx_t n)
{
    /* Memoization: store all previously computed terms. */
    static seq_t *terms = NULL;                /* a_vals array */
    static size_t terms_cap = 0;               /* allocated capacity */
    static idx_t current_max = 0;              /* highest precomputed index */
    static seq_t f = 0;                        /* (current_k + 1)! value for next term */
    static idx_t f_k = 0;                      /* index for which f holds (f_k + 1)! */

    /* Ensure the a(0) value is initialized. */
    if (current_max == 0 && terms == NULL) {
        terms_cap = 1;
        terms = (seq_t *)malloc(terms_cap * sizeof(seq_t));
        if (terms == NULL) return 0;          /* allocation failure, return 0 */
        terms[0] = 0;                         /* a(0) = 0 */
        f = (seq_t)2;                         /* 2! for k = 1 */
        f_k = 0;
        current_max = 0;
    }

    /* Raise capacity if necessary. */
    if (n + 1 > terms_cap) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *new_terms = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
        if (new_terms == NULL) return 0;      /* allocation failure, return 0 */
        terms = new_terms;
        terms_cap = new_cap;
    }

    /* Compute up to requested index if needed. */
    if (n > current_max) {
        /* Starting from k = current_max + 1 ... n inclusive. */
        for (idx_t k = current_max + 1; k <= n; ++k) {
            /* term = k² * (k+1)! ; f currently holds (k+1)! */
            seq_t k_sq = (seq_t)k * (seq_t)k;
            seq_t term = k_sq * f;
            terms[k] = terms[k - 1] + term;

            /* Advance factorial for next k.  (k+1)! -> (k+2)!   */
            f *= (seq_t)(k + 2);
            f_k = k;
        }
        current_max = n;
    }

    return terms[n];
}
