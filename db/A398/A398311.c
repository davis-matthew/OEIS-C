/*
 * A398311  G.f. A(x) satisfies: A(x) = 1 + x * A(x/(1 - 2*x)^2) / (1 - x).
 * https://oeis.org/A398311
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, new
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS sequence A398311
   Formula: a(n) = 1 + Σ_{k=1..n-1} Σ_{j=0..n-k-1} binomial(j+2*k-1,j) * 2^j * a(k)
   Offset: 0   (a(0) is the first term)
*/
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Allocate arrays for dynamic programming */
    seq_t *a_seq   = (seq_t*)malloc((n + 1) * sizeof(seq_t));
    seq_t *sum_b   = (seq_t*)calloc((n + 1), sizeof(seq_t));   /* cumulative sums B(k,jmax) */
    seq_t *term    = (seq_t*)calloc((n + 1), sizeof(seq_t));   /* current term_j for each k */
    idx_t *curr_j  = (idx_t*)calloc((n + 1), sizeof(idx_t));   /* last computed j for each k */

    if (!a_seq || !sum_b || !term || !curr_j) {
        /* Allocation failed – return 0 */
        return (seq_t)0;
    }

    /* Initial values */
    a_seq[0] = (seq_t)1;
    if (n == 0) {
        seq_t res = a_seq[0];
        free(a_seq);
        free(sum_b);
        free(term);
        free(curr_j);
        return res;
    }

    a_seq[1] = (seq_t)1;
    /* initialise cumulative sums and terms for k >= 1 */
    for (idx_t k = 1; k <= n; ++k) {
        sum_b[k] = (seq_t)1;   /* term j=0 */
        term[k]  = (seq_t)1;   /* term j=0 */
        curr_j[k] = 0;         /* current j */
    }

    /* Main recurrence */
    for (idx_t nn = 2; nn <= n; ++nn) {
        seq_t total = (seq_t)0;

        for (idx_t k = 1; k < nn; ++k) {
            /* maximum j needed for this k at current n */
            idx_t need = nn - k - 1;          /* need >= 0 */

            /* Extend term and sum_b for current k up to 'need' */
            while (curr_j[k] < need) {
                ++curr_j[k];                  /* new j (>=1) */

                /* term_j = term_{j-1} * (j+2k-1)/j * 2 */
                seq_t num = term[k] *
                            (((seq_t)curr_j[k] + 2 * (seq_t)k) - (seq_t)1);
                seq_t div = curr_j[k];
                seq_t tmp = num / div;
                term[k]  = tmp * (seq_t)2;

                sum_b[k] += term[k];
            }

            /* Accumulate contribution of this k */
            total += sum_b[k] * a_seq[k];
        }

        a_seq[nn] = (seq_t)1 + total;
    }

    seq_t result = a_seq[n];

    /* Clean up */
    free(a_seq);
    free(sum_b);
    free(term);
    free(curr_j);

    return result;
}
