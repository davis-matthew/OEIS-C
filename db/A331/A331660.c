/*
 * A331660  E.g.f. A(x) satisfies: d/dx A(x) = 1 + (1/(1 - x)) * A(x/(1 - x)).
 * https://oeis.org/A331660
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; a(n+1) = Σ_{k=0}^{n-1} C(n,k)^2 * k! * a(n-k) (for n ≥
 *            1)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 10, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* External integer types supplied by the build */
extern seq_t a(idx_t n);

seq_t a(idx_t n)
{
    /* Cache for the sequence and factorials, indexed from 0 */
    static seq_t *seq   = NULL;      /* a[1]..a[n]  (a[0] unused) */
    static seq_t *fact  = NULL;      /* fact[0]..fact[n]  */
    static idx_t  seq_cap  = 0;      /* allocated capacity for seq   */
    static idx_t  fact_cap = 0;      /* allocated capacity for fact  */
    static idx_t  seq_last = 0;      /* highest index computed in seq */
    static idx_t  fact_last = 0;     /* highest index computed in fact */

    /* The sequence is defined for n ≥ 1; return 0 for n < 1 (unused) */
    if (n < 1) return (seq_t)0;

    /* Ensure seq has space for indices up to n (slot 0 unused) */
    if (seq_cap <= n) {
        idx_t new_cap = seq_cap ? seq_cap * 2 : 4;
        if (new_cap <= n) new_cap = n + 1;
        seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        seq_cap = new_cap;
    }

    /* Ensure fact has space for factorials up to n */
    if (fact_cap <= n) {
        idx_t new_cap = fact_cap ? fact_cap * 2 : 4;
        if (new_cap <= n) new_cap = n + 1;
        fact = (seq_t *)realloc(fact, new_cap * sizeof(seq_t));
        fact_cap = new_cap;
    }

    /* Initialise base cases on first call */
    if (seq_last == 0) {
        seq[0]   = (seq_t)0;  /* unused */
        seq[1]   = (seq_t)1;  /* a(1) = 1 */
        fact[0]  = (seq_t)1;  /* 0! = 1 */
        fact[1]  = (seq_t)1;  /* 1! = 1 */
        seq_last = 1;
        fact_last = 1;
    }

    /* Extend factorial table up to n */
    while (fact_last < n) {
        fact_last++;
        fact[fact_last] = fact[fact_last - 1] * (seq_t)fact_last;
    }

    /* Compute sequence values up to n if not already done */
    while (seq_last < n) {
        idx_t m = seq_last;          /* compute a(m+1) using recurrence with n = m */
        seq_t sum   = (seq_t)0;
        seq_t binom = (seq_t)1;      /* C(m,0) */

        for (idx_t k = 0; k < m; ++k) {
            /* C(m,k)^2 * k! * a(m-k) */
            seq_t binom_sq = binom * binom;
            seq_t term     = binom_sq * fact[k];
            term           = term * seq[m - k];
            sum += term;

            /* Update binom to C(m,k+1) if needed */
            if (k + 1 < m) {
                binom = binom * (seq_t)(m - k) / (seq_t)(k + 1);
            }
        }

        seq[m + 1] = sum;
        seq_last = m + 1;
    }

    return seq[n];
}
