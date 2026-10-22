/*
 * A328436  Number of inversion sequences of length n avoiding the consecutive
 *          patterns 000 and 001.
 * https://oeis.org/A328436
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 13, u64 n <= 21, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: Let
 *   E[k][v] = number of inversion sequences of length k whose last two
 *             entries are equal: e_{k-1} = e_k = v.
 *   N[k][v] = number of inversion sequences of length k whose last
 *             entry is v but e_{k-1} != v.
 *
 * For k ≥ 3 the transition from length k−1 to k is
 *
 *   N[k][r] = (∑_v N[k-1][v] − N[k-1][r])           // previous pair not equal
 *             + ∑_{v>r} E[k-1][v]                  // previous pair equal, r < v
 *
 *   E[k][r] = N[k-1][r]                              // equal pair only comes
 *             from a previous non‑equal pair
 *
 * Initial values for length 2:
 *   E[2][0] = 1   (sequence 00)
 *   N[2][1] = 1   (sequence 01)
 *
 * The desired value a(n) = Σ_v (E[n][v] + N[n][v]).
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Small n are trivial */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;

    /* Allocate arrays of size n (values 0 .. n‑1 are possible) */
    seq_t *eq   = (seq_t*)calloc(n, sizeof(seq_t));   /* E */
    seq_t *neq  = (seq_t*)calloc(n, sizeof(seq_t));   /* N */
    seq_t *eq_new   = (seq_t*)calloc(n, sizeof(seq_t));
    seq_t *neq_new  = (seq_t*)calloc(n, sizeof(seq_t));
    seq_t *suff  = (seq_t*)calloc(n, sizeof(seq_t));
    if (!eq || !neq || !eq_new || !neq_new || !suff) return (seq_t)0; /* oops */

    /* Length 2 initialization */
    eq[0]  = (seq_t)1;     /* 00  */
    neq[1] = (seq_t)1;     /* 01  */

    /* DP for lengths 3 .. n */
    for (idx_t len = 3; len <= n; ++len) {
        /* total number of N entries for values < len-1 */
        seq_t total_neq = (seq_t)0;
        for (idx_t i = 0; i < len - 1; ++i) total_neq += neq[i];

        /* suffix sums of eq for values > r */
        seq_t cum = (seq_t)0;
        for (idx_t i = len - 2; ; --i) {
            cum += eq[i];
            suff[i] = cum;
            if (i == 0) break;
        }

        /* Compute new arrays */
        for (idx_t r = 0; r < len; ++r) {
            /* N transition */
            seq_t nprev = (r < len - 1) ? neq[r] : (seq_t)0;
            seq_t sumE  = (r + 1 <= len - 2) ? suff[r + 1] : (seq_t)0;
            neq_new[r] = (total_neq - nprev) + sumE;

            /* E transition */
            eq_new[r] = nprev;
        }

        /* Swap pointers for next iteration */
        seq_t *tmp = eq;   eq   = eq_new;   eq_new   = tmp;
        tmp = neq;  neq  = neq_new;  neq_new  = tmp;
    }

    /* Sum up all valid sequences of length n */
    seq_t result = (seq_t)0;
    for (idx_t v = 0; v < n; ++v)
        result += eq[v] + neq[v];

    /* Free allocated memory */
    free(eq);    free(neq);
    free(eq_new); free(neq_new);
    free(suff);

    return result;
}
