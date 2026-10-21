/*
 * A294922  Expansion of 1/(1 + x/(1 + x^3/(1 + x^4/(1 + x^7/(1 + x^11/(1 +
 *          ... + x^Lucas(k)/(1 + ...))))))), a continued fraction.
 * https://oeis.org/A294922
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 16, i16 n <= 32, i32 n <= 45, i64 n <= 45, i128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A294922:
   a(n) is the coefficient of xⁿ in the continued fraction
   1 / (1 + x/(1 + x³/(1 + x⁴/(1 + x⁷/(1 + ...))))),
   where the exponents are Lucas numbers 1, 3, 4, 7, 11, ….
   This implementation builds the coefficient series up to the
   required index using only the operations allowed in the
   GENESIS framework. */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* n = 0 gives the first term directly */
    if (n == (idx_t)0)
        return (seq_t)1;

    /* ---------- 1. Build list of Lucas exponents <= n ----------
       The Lucas numbers start 2,1,3,4,7,11,…; the first term of
       the continued fraction uses exponent 1, then the remaining
       Lucas numbers. ------------------------------------------------ */
    size_t exps_capacity = 64;
    idx_t *exps = (idx_t *)malloc(exps_capacity * sizeof(idx_t));
    if (!exps) return (seq_t)0;      /* allocation failure */

    size_t exps_cnt = 0;
    exps[exps_cnt++] = 1;            /* first exponent */

    idx_t l0 = 2, l1 = 1;            /* L0=2, L1=1 */
    while (1) {
        idx_t nxt = l0 + l1;
        if (nxt > n) break;
        if (exps_cnt == exps_capacity) {
            exps_capacity *= 2;
            idx_t *tmp = (idx_t *)realloc(exps, exps_capacity * sizeof(idx_t));
            if (!tmp) { free(exps); return (seq_t)0; }
            exps = tmp;
        }
        exps[exps_cnt++] = nxt;
        l0 = l1;
        l1 = nxt;
    }

    /* ---------- 2. Allocate coefficient arrays ----------
       A holds the current tail coefficients.  B is temporary. ----- */
    size_t sz = (size_t)n + 1;
    seq_t *A = (seq_t *)calloc(sz, sizeof(seq_t));
    seq_t *B = (seq_t *)calloc(sz, sizeof(seq_t));
    if (!A || !B) {
        free(exps);
        free(A);
        free(B);
        return (seq_t)0;
    }

    A[0] = (seq_t)1;                 /* tail beyond the deepest exponent is 1 */

    /* ---------- 3. Process exponents in reverse (deepest first) ----- */
    for (size_t rev = 0; rev < exps_cnt; ++rev) {
        idx_t e = exps[exps_cnt - 1 - rev];   /* current exponent */

        /* Zero B, then set its constant term */
        memset(B, 0, sz * sizeof(seq_t));
        B[0] = (seq_t)1;

        /* Compute B[k] = - Σ_{t=0}^{k-e} A[t] * B[k-e-t] for k ≥ e */
        for (size_t k = (size_t)e; k < sz; ++k) {
            seq_t sum = (seq_t)0;
            for (size_t t = 0; t <= k - e; ++t) {
                sum += A[t] * B[(size_t)(k - e - t)];
            }
            B[k] = -sum;
        }

        /* Copy B into A for the next iteration */
        memcpy(A, B, sz * sizeof(seq_t));
    }

    /* ---------- 4. Result ----------
       The coefficient of xⁿ is now A[n].                   */
    seq_t result = A[(size_t)n];

    /* ---------- 5. Clean up ----------
       Free all dynamically allocated memory.    */
    free(A);
    free(B);
    free(exps);

    return result;
}
