/*
 * A306900  a(n) is the total number of sum of weighted records over set
 *          partitions of [n].
 * https://oeis.org/A306900
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*(B(n+3)-B(n+2))/4 - (n+7/4)*B(n+1) - (n+1)*B(n)/2
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 22, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where B(k) is the k‑th Bell number (A000110). */
/* The function computes a(n) for any n ≥ 0 using memoised Bell numbers. */
#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Cached Bell numbers: bell[i] = B_i.  Indexed from 0. */
    static seq_t *bell = NULL;
    static size_t bell_capacity = 0;    /* allocated capacity */
    static size_t bell_length  = 0;     /* next unused index */

    idx_t max_needed = n + 3;           /* we need B_{n+3} for a(n) */

    /* Initialise or grow the cache so that we have space up to max_needed. */
    if (bell_capacity <= (size_t)max_needed) {
        size_t new_cap = (size_t)max_needed + 1;
        seq_t *tmp = (seq_t *)realloc(bell, new_cap * sizeof(seq_t));
        if (tmp == NULL) abort();      /* allocation failure */
        bell = tmp;
        bell_capacity = new_cap;
    }

    /* Initialise the very first value; B_0 = 1. */
    if (bell_length == 0) {
        bell[0] = (seq_t)1;
        bell_length = 1;
    }

    /* Compute Bell numbers up to max_needed if necessary. */
    for (idx_t i = bell_length; i <= max_needed; ++i) {
        seq_t sum   = (seq_t)0;
        seq_t binom = (seq_t)1;                /* C(i-1, 0) = 1 */
        for (idx_t k = 0; k < i; ++k) {
            sum += binom * bell[k];
            /* Update binomial coefficient for next k */
            if (k + 1 < i) {
                seq_t num = (seq_t)(i - 1 - k); /* i-1-k  */
                seq_t den = (seq_t)(k + 1);    /* k+1    */
                binom = (binom * num) / den;
            }
        }
        bell[i] = sum;
    }
    bell_length = max_needed + 1;           /* now we have B_0 … B_{max_needed} */

    /*  Compute the formula using integer arithmetic. */
    seq_t term1 = bell[n + 3] - bell[n + 2];
    term1 = term1 * (seq_t)3;

    seq_t n_seq   = (seq_t)n;
    seq_t term2   = bell[n + 1];
    term2 = term2 * ((seq_t)4 * n_seq + (seq_t)7);

    seq_t term3   = bell[n];
    term3 = term3 * ( (n_seq + (seq_t)1) * (seq_t)2 );

    seq_t numerator = term1 - term2 - term3;
    seq_t result    = numerator / (seq_t)4;

    return result;
}
