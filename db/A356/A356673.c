/*
 * A356673  a(n) = n! * Sum_{k=0..n} k^(3*(n-k))/k!.
 * https://oeis.org/A356673
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 11, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = n! * Sum_{k=0..n} k^(3*(n-k))/k!
 *
 * This implementation computes the sequence by iteratively building the
 * terms d_k(n) = n! / k! * k^(3*(n-k)).  For each new index n the
 * previously computed terms are multiplied by (n * k^3) and a new term
 * d_n(n) = 1 is added.  The sum of all d_k(n) for k=1..n gives a(n)
 * (for n>=1; a(0)=1).  This requires O(n^2) multiplications/additions
 * which is fast enough for n up to about 10 000 on the available
 * integer sizes.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static state for memoisation */
    static seq_t *term = NULL;     /* term[k] = d_k(current_n)  (k>=1) */
    static size_t term_cap = 0;    /* allocated capacity for term array */
    static idx_t term_n   = 0;     /* largest n that has been precomputed */

    static seq_t *aval  = NULL;    /* a values, aval[0] = 1 */
    static size_t aval_cap = 0;    /* allocated capacity for aval array */

    /* initialise storage on first call */
    if (aval == NULL) {
        aval_cap = 2;                    /* at least space for a0 */
        aval = (seq_t*)malloc(aval_cap * sizeof(seq_t));
        aval[0] = (seq_t)1;
    }
    if (term == NULL) {
        term_cap = 4;                    /* small array for term[1..] */
        term = (seq_t*)malloc(term_cap * sizeof(seq_t));
    }

    if (n == 0) return aval[0];

    /* grow a-values array if necessary */
    if (aval_cap <= (size_t)n) {
        size_t new_cap = (aval_cap + 1) * (size_t)2;
        aval = (seq_t*)realloc(aval, new_cap * sizeof(seq_t));
        aval_cap = new_cap;
    }

    /* grow terms array if necessary */
    if (term_cap <= (size_t)(n + 1)) {
        size_t new_cap = (term_cap + n + 1) * (size_t)2;
        term = (seq_t*)realloc(term, new_cap * sizeof(seq_t));
        term_cap = new_cap;
    }

    /* start from the next index that needs computing */
    while (term_n < n) {
        idx_t cur = term_n + 1;                 /* new index to compute */

        /* update previous terms: term[k] *= cur * k^3  for 1<=k<cur */
        for (idx_t k = 1; k < cur; ++k) {
            seq_t ksq = (seq_t)k * (seq_t)k;
            seq_t k3  = ksq * (seq_t)k;
            seq_t mul = (seq_t)cur * k3;
            term[k] *= mul;
        }

        /* new term for k = cur : d_cur(cur) = 1 */
        term[cur] = (seq_t)1;

        /* sum all terms to obtain a(cur) */
        seq_t sum = (seq_t)0;
        for (idx_t k = 1; k <= cur; ++k) {
            sum += term[k];
        }
        aval[cur] = sum;

        term_n = cur;
    }

    return aval[n];
}
