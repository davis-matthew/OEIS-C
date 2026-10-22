/*
 * A364627  G.f. satisfies A(x) = 1/(1-x)^3 + x^2*A(x)^4.
 * https://oeis.org/A364627
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..⌊n/2⌋} binomial(n+7*k+2, 9*k+2) * binomial(4*k,
 *            k) / (3*k+1) This implementation uses the provided
 *            binomial(idx_t n, idx_t k) helper
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"   /* assumes binomial function is declared here */

seq_t a(idx_t n) {
    /* Ensure n is a non‑negative index (offset 0). */
    if ((int64_t)n < 0) return 0;  /* undefined for negative n, return 0 */

    seq_t result = 0;
    idx_t max_k = n >> 1;          /* floor(n/2) */

    for (idx_t k = 0; k <= max_k; ++k) {
        /* Compute the two binomial coefficients. */
        idx_t n1 = n + 7 * k + 2;
        idx_t k1 = 9 * k + 2;
        seq_t term1 = binomial(n1, k1);

        idx_t n2 = 4 * k;
        idx_t k2 = k;
        seq_t term2 = binomial(n2, k2);

        /* Multiply them and divide by (3*k + 1). */
        seq_t term = term1 * term2;
        term /= (seq_t)(3 * k + 1);

        result += term;
    }

    return result;
}
