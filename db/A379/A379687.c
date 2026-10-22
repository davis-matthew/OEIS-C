/*
 * A379687  Expansion of e.g.f. (1/x) * Series_Reversion( x * exp(x) * (1 -
 *          2*x*exp(x)) ).
 * https://oeis.org/A379687
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/(n+1)) * \sum_{k=0..n} 2^{\,n-k} * (-k-1)^k *
 *            (2n-k)!/(k! (n-k)!)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 17,
 *            big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation done directly from the above combinatorial sum. */
/* Author: ChatGPT */

#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    // Handle n == 0 immediately
    if (n == 0u) {
        return (seq_t)1;
    }

    // Compute A_0 = product_{j=1}^{n} (n+j)  = (2n)! / n!
    seq_t A = (seq_t)1;
    for (idx_t j = 1; j <= n; ++j) {
        A *= (seq_t)(n + j);
    }

    // pow2 = 2^{n}
    seq_t pow2 = pow_seq((seq_t)2, (int)n);

    // Sum of terms with sign
    seq_t sum_val = (seq_t)0;
    int    sum_sign = 0;   // 0 = zero, 1 = positive, -1 = negative

    for (idx_t k = 0; k <= n; ++k) {
        // Compute (k+1)^k
        seq_t pow_kp1 = pow_seq((seq_t)(k + 1), (int)k);

        // term = pow2 * pow_kp1 * A
        seq_t term = pow2 * pow_kp1;
        term = term * A;

        int sign = (k & 1u) ? -1 : 1;  // (-1)^k

        // Signed addition to the running total
        if (sum_sign == 0) {
            sum_val  = term;
            sum_sign = sign;
        } else if (sum_sign == sign) {
            sum_val += term;
        } else {
            if (term > sum_val) {
                sum_val  = term - sum_val;
                sum_sign = sign;
            } else {
                sum_val -= term;
                /* sign stays as before */
            }
        }

        // Prepare for next iteration (if any)
        if (k < n) {
            // Update pow2: pow2 = 2^{n-(k+1)} = pow2 / 2
            pow2 = pow2 / 2;

            // Update A: A_{k+1} = A_k * (n-k) / ((k+1)*(2n-k))
            seq_t numerator = A * (seq_t)(n - k);
            seq_t denom     = ((seq_t)(k + 1) * (seq_t)(2 * n - k));
            A = numerator / denom;
        }
    }

    // Divide by (n+1)
    seq_t result = sum_val / (seq_t)(n + 1);
    return result;
}
