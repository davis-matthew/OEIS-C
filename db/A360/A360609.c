/*
 * A360609  E.g.f. satisfies A(x) = exp(x*A(x)^3) / (1-x).
 * https://oeis.org/A360609
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=0..n} (3*k+1)^(k-1) * binomial(n+2*k, n-k) /
 *            k!
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 16,
 *            big any n
 * Helpers:   binomial, factorial, pow_seq_large
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation follows the direct summation of the series.
 * It uses the provided big‑integer helpers:
 *   binomial(idx_t n, idx_t k)
 *   factorial(idx_t n)
 *   pow_seq_large(seq_t base, seq_t exp)
 *
 * For each term in the sum we compute:
 *   X_k = n! / k!             (integer)
 *   P_k = (3*k+1)^(k-1)       (big‑integer power)
 *   C_k = binomial(n+2*k, n-k) (computed incrementally)
 *   term = X_k * P_k * C_k
 *   sum += term
 * The sequence is returned as the accumulated sum.
 */

#include "../utils/binomial.h"
#include "../utils/factorial.h"
#include "../utils/pow_seq_large.h"

seq_t a(idx_t n)
{
    /* pre‑compute n! once */
    seq_t nfact = factorial(n);

    /* k! for current k, initialized to 0! = 1 */
    seq_t fact_k = 1;

    /* binomial term C_k = binomial(n+2*k, n-k) for current k,
     * starting with C_0 = binomial(n, n) = 1 */
    seq_t binom = 1;

    seq_t sum = (seq_t)0;

    for (idx_t k = 0; k <= n; ++k) {
        /* X_k = n! / k!   (exact division) */
        seq_t Xk = nfact / fact_k;

        /* P_k = (3*k+1)^(k-1); for k==0 we define P_0 = 1 */
        seq_t Pk;
        if (k == 0) {
            Pk = (seq_t)1;
        } else {
            seq_t base = (seq_t)(3 * k + 1);
            seq_t exp  = (seq_t)(k - 1);
            Pk = pow_seq_large(base, exp);
        }

        /* term = C_k * P_k * X_k */
        seq_t term = binom * Pk;
        term *= Xk;

        sum += term;

        /* Prepare next iteration values if k < n */
        if (k < n) {
            /* Update factorial part: (k+1)! */
            fact_k *= (k + 1);

            /* Update binomial term via recurrence:
             * C_{k+1} = C_k * (n+2*k+2)(n+2*k+1)(n-k) / ((3*k+3)(3*k+2)(3*k+1)) */
            idx_t num1_idx = n + 2 * k + 2;
            idx_t num2_idx = n + 2 * k + 1;
            idx_t num3_idx = n - k;
            idx_t den1_idx = 3 * k + 3;
            idx_t den2_idx = 3 * k + 2;
            idx_t den3_idx = 3 * k + 1;

            seq_t temp = binom;
            temp *= (seq_t)num1_idx;
            temp *= (seq_t)num2_idx;
            temp *= (seq_t)num3_idx;
            temp /= (seq_t)den1_idx;
            temp /= (seq_t)den2_idx;
            temp /= (seq_t)den3_idx;
            binom = temp;
        }
    }

    return sum;
}
