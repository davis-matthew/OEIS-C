/*
 * A395023  Expansion of the o.g.f. Sum_{k >= 0} ((k + 1) * x^k) /
 *          Product_{j=1..k} (1 - j^2 * x).
 * https://oeis.org/A395023
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in Σ_{k≥0} ((k+1)*x^k) /* /∏_{j=1..k}
 *            (1 - j² * x)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

#if defined(__cplusplus)
/* In C++ use new/delete to ensure construction/destruction of big‐int types */
#define SEQ_ALLOC(n)   new seq_t[n]
#define SEQ_FREE(p)    delete[] (p)
#else
/* In C use calloc/free – works for plain integer types */
#define SEQ_ALLOC(n)   ((seq_t*)calloc((n), sizeof(seq_t)))
#define SEQ_FREE(p)    free(p)
#endif

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* Allocate space for the coefficient arrays */
    seq_t *a_coeff = SEQ_ALLOC(n + 1);
    seq_t *c_coeff = SEQ_ALLOC(n + 1);
    if (!a_coeff || !c_coeff) {
        /* allocation failed – return 0 (should not happen in tests) */
        return (seq_t)0;
    }

    /* initialise to zero */
    for (idx_t i = 0; i <= n; ++i) {
        a_coeff[i] = (seq_t)0;
        c_coeff[i] = (seq_t)0;
    }
    a_coeff[0] = (seq_t)1;  /* a(0) = 1 */
    c_coeff[0] = (seq_t)1;  /* 1/D_0 = 1 */

    /* Main loop: for each k, add (k+1)*x^k / ∏_{j=1..k}(1 - j²x) */
    for (idx_t k = 0; k <= n; ++k) {
        seq_t factor = (seq_t)(k + 1);
        seq_t mult   = factor * factor;           /* (k+1)^2 */
        idx_t max_m  = n - k;                     /* highest m with k+m ≤ n */

        /* Add contributions to a_coeff */
        for (idx_t m = 0; m <= max_m; ++m) {
            a_coeff[k + m] += factor * c_coeff[m];
        }

        if (k == n)
            break;                                 /* no next c available */

        /* Update c_coeff to represent 1 / ∏_{j=1..k+1} (1 - j²x) */
        idx_t limit = n - (k + 1);
        for (idx_t m = 1; m <= limit; ++m) {
            seq_t old = c_coeff[m];
            c_coeff[m] = old + mult * c_coeff[m - 1];
        }
        /* c_coeff[0] remains 1 */
    }

    seq_t result = a_coeff[n];

    SEQ_FREE(a_coeff);
    SEQ_FREE(c_coeff);

    return result;
}
