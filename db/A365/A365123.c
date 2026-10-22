/*
 * A365123  G.f. satisfies A(x) = (1 + x / (1 - x*A(x))^4)^2.
 * https://oeis.org/A365123
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(2*(n-k+1), k) * binomial(n+3*k-1,
 *            n-k) / (n-k+1) (OEIS A365123)
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 10, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/binomial.h"   // binomial(idx_t n, idx_t k)

seq_t a(idx_t n)
{
    if (n == 0)  /* a(0) = 1 */
        return (seq_t)1;

    seq_t result = 0;
    seq_t bin1 = 1;                       /* binomial(2*(n+1), 0) */
    seq_t bin2 = 0;                       /* k = 0 term is zero for n>0 */

    /* k = 0 term contributes 0 for n > 0 */
    /* Now iterate k from 1 to n */
    for (idx_t k = 1; k <= n; ++k) {
        /* Update bin1: binomial(2*(n-k+1), k) from previous k-1 */
        seq_t Mk = (seq_t)(2 * ( (seq_t)(n - k + 2) ));              /* top of previous binomial */
        seq_t num1 = (Mk - (k - 1)) * (Mk - (k - 1) - (seq_t)1) *
                     (Mk - (k - 1) - (seq_t)2);
        seq_t den1 = Mk * (Mk - (seq_t)1) * (seq_t)k;
        bin1 = bin1 * num1 / den1;

        /* Update bin2 */
        if (k == 1) {
            /* First non‑zero term: binomial(n+2, n-1) */
            bin2 = binomial((idx_t)(n + 2), (idx_t)(n - 1));
        } else {
            seq_t N_prev = (seq_t)(n + 3 * (k - 1) - 1);   /* N_{k-1} */
            seq_t R_prev = (seq_t)(n - (k - 1));          /* R_{k-1} */
            seq_t num2 = (N_prev + (seq_t)1) * (N_prev + (seq_t)2) *
                         (N_prev + (seq_t)3) * R_prev;
            seq_t den2 = (N_prev - R_prev + (seq_t)1) *
                         (N_prev - R_prev + (seq_t)2) *
                         (N_prev - R_prev + (seq_t)3) *
                         (N_prev - R_prev + (seq_t)4);
            bin2 = bin2 * num2 / den2;
        }

        /* Add term to result */
        seq_t denom = (seq_t)(n - k + 1);
        seq_t term = bin1 * bin2 / denom;
        result += term;
    }

    return result;
}
