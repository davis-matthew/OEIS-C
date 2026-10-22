/*
 * A351514  Expansion of e.g.f. (exp(exp(exp(exp(x)-1)-1)-1)-1)^2 / 2.
 * https://oeis.org/A351514
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n-1} binomial(n-1,k) * A000307(k) *
 *            A000307(n-k)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 19,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (offset 2, a(2)=1, a(3)=12, ...) */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/binomial.h"

extern seq_t binomial(idx_t n, idx_t k);

static idx_t max_n_global = 0;
static seq_t *B = NULL;   /* coefficients for exp(exp(x)-1)      */
static seq_t *C = NULL;   /* coefficients for exp(S2)-1          */
static seq_t *D = NULL;   /* coefficients for exp(S3)-1          */

seq_t a(idx_t n)
{
    if (n < 2) return 0;

    /* enlarge arrays if necessary and compute new terms incrementally */
    if (n > max_n_global) {
        idx_t old_n = max_n_global;
        max_n_global = n;

        /* reallocate tables */
        B = (seq_t*)realloc(B, (max_n_global + 1) * sizeof(seq_t));
        C = (seq_t*)realloc(C, (max_n_global + 1) * sizeof(seq_t));
        D = (seq_t*)realloc(D, (max_n_global + 1) * sizeof(seq_t));

        /* initialise new entries to zero */
        for (idx_t i = old_n + 1; i <= max_n_global; ++i) {
            B[i] = (seq_t)0;
            C[i] = (seq_t)0;
            D[i] = (seq_t)0;
        }

        /* B[0] = 1 for exp(exp(x)-1) */
        if (old_n == 0) B[0] = (seq_t)1;
        /* compute B up to n */
        for (idx_t i = old_n + 1; i <= max_n_global; ++i) {
            seq_t sum = (seq_t)0;
            for (idx_t k = 1; k <= i; ++k) {
                seq_t term = binomial(i - 1, k - 1);
                term *= B[i - k];
                sum += term;
            }
            B[i] = sum;
        }

        /* C[0] = 1 for exp(S2)-1 */
        if (old_n == 0) C[0] = (seq_t)1;
        /* compute C up to n */
        for (idx_t i = old_n + 1; i <= max_n_global; ++i) {
            seq_t sum = (seq_t)0;
            for (idx_t k = 1; k <= i; ++k) {
                seq_t term = binomial(i - 1, k - 1);
                term *= B[k];       /* s_k = B_k */
                term *= C[i - k];
                sum += term;
            }
            C[i] = sum;
        }

        /* D[0] = 1 for exp(S3)-1 */
        if (old_n == 0) D[0] = (seq_t)1;
        /* compute D up to n */
        for (idx_t i = old_n + 1; i <= max_n_global; ++i) {
            seq_t sum = (seq_t)0;
            for (idx_t k = 1; k <= i; ++k) {
                seq_t term = binomial(i - 1, k - 1);
                term *= C[k];       /* s_k = C_k */
                term *= D[i - k];
                sum += term;
            }
            D[i] = sum;
        }
    }

    /* S4 coefficients: S4[0] = 0, S4[n] = D[n] for n>=1 */
    seq_t sum = (seq_t)0;
    for (idx_t k = 1; k < n; ++k) {
        seq_t term = binomial(n, k);
        term *= D[k];
        term *= D[n - k];
        sum += term;
    }
    /* divide by 2 (exact division) */
    return sum / (seq_t)2;
}
