/*
 * A378091  E.g.f. satisfies A(x) = exp(x * (1-x)^3 * A(x)) / (1-x)^4.
 * https://oeis.org/A378091
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 18,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A378091
   Formula (as derived from the EGF):
   a(n) = Σ_{k=0..n} (k+1)^(k-1) * C(n+3, n-k) * (n! / k!)
   This implementation evaluates the sum in O(n) time using
   incremental updates for the factorial ratio and the binomial
   coefficients.  The power (k+1)^(k-1) is computed by the provided
   utility pow_uint, which returns a big integer value. */

#include <stdbool.h>
#include "../utils/pow_uint.h"

/* Computes the n-th term of the sequence A378091. */
seq_t a(idx_t n)
{
    /* Special case: for n = 0 the formula gives a(0) = 1 directly. */
    if (n == 0) return (seq_t)1;

    /* sum accumulates the final result */
    seq_t sum = 0;

    /* P holds the product ∏_{t=k+1}^{n} t, starting with 1 for k = n. */
    seq_t P = 1;

    /* binomC holds the binomial coefficient C(n+3, n-k).
       Initially for k = n we have C(n+3, 0) = 1. */
    seq_t binomC = 1;

    /* Iterate k from n down to 0.  At each step we evaluate
       term_k = (k+1)^(k-1) * binomC * P and add it to sum. */
    for (idx_t k = n; ; --k) {
        seq_t pow_term;
        if (k <= 1) {
            /* (k+1)^(k-1) is 1 for k = 0 or 1 */
            pow_term = (seq_t)1;
        } else {
            pow_term = pow_uint((uint64_t)(k + 1), k - 1);
        }

        seq_t term = pow_term;
        term *= binomC;
        term *= P;

        sum += term;

        if (k == 0) break;   /* finished all terms */

        /* Update P for the next iteration: P *= k */
        P *= (seq_t)k;

        /* Update binomC for the next iteration:
           C(n+3, n-(k-1)) = C(n+3, n-k) * (k+3) / (n-k+1)   */
        seq_t num = (seq_t)(k + 3);
        seq_t den = (seq_t)(n - k + 1);
        binomC = binomC * num / den;
    }

    return sum;
}
