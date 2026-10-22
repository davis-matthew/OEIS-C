/*
 * A367331  a(n) = 27^n * Sum_{k=0..n} (-1)^k*binomial(1/3, k)^2.
 * https://oeis.org/A367331
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 27^n * Sum_{k=0..n} (-1)^k*binomial(1/3, k)^2
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 16,
 *            big any n
 * Helpers:   gcd, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow3.h"
#include "../utils/gcd.h"

/**
 * Compute a(n) from OEIS A367331.
 * Uses the recurrence
 *   A_k = -A_{k-1} * (3k-4)^2 / (9*k^2),
 * where A_k = 27^n * (-1)^k * binom(1/3, k)^2.
 * The absolute values satisfy
 *   |A_k| = |A_{k-1}| * (3k-4)^2 / (9*k^2).
 * The sign of A_k is (-1)^k.
 * Since a(n) = Σ_{k=0}^n A_k and the partial sums are always non‑negative,
 * we accumulate positive and negative parts separately to avoid temporary
 * negativity and to keep the algorithm working for all supported seq_t types.
 */
seq_t a(idx_t n) {
    /* Base term A_0 = 27^n */
    seq_t absA = pow3(3 * n);

    /* Separate accumulators for positive and negative contributions */
    seq_t posSum = absA;   /* k = 0 contributes positively */
    seq_t negSum = 0;

    for (idx_t k = 1; k <= n; ++k) {
        /* Denominator: 9*k^2 */
        idx_t k2 = k * k;
        seq_t denom = (seq_t)(9 * k2);

        /* Numerator factor: (3k-4)^2  (positive integer) */
        int64_t tmp = 3 * (int64_t)k - 4;
        seq_t multSq = (seq_t)(tmp * tmp);

        /* Reduce intermediate division to avoid overflow:
         *   absA_new = absA * multSq / denom
         *   We cancel common factors between absA/multSq and denom.
         */
        seq_t g1 = gcd(absA, denom);
        absA /= g1;
        denom /= g1;

        seq_t g2 = gcd(multSq, denom);
        multSq /= g2;
        denom /= g2;

        /* Perform the multiplication and division */
        absA = absA * multSq / denom;

        /* Update the running sum according to the sign (-1)^k */
        if ((k & 1ULL) == 0) {
            posSum += absA;   /* even k ⇒ positive */
        } else {
            negSum += absA;   /* odd k ⇒ negative */
        }
    }

    /* Result is the difference of positive and negative parts */
    return posSum - negSum;
}
