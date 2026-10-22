/*
 * A380512  Expansion of e.g.f. exp(x*G(x)^3) where G(x) = 1 + x*G(x)^3 is the
 *          g.f. of A001764.
 * https://oeis.org/A380512
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0}^{n-1} (∏_{i=0}^{k-1} (n-1-i)) · C(3n, k), where C
 *            is the binomial coefficient. The function computes this sum
 *            iteratively using only integer arithmetic
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>  /* needed by the build system, even if unused */

seq_t a(idx_t n) {
    if (n == 0) {                /* offset is 0, a(0)=1 */
        return (seq_t)1;
    }

    seq_t sum   = 0;                /* running sum of the terms */
    seq_t binom = 1;                /* C(3n,0) = 1 */
    seq_t prod  = 1;                /* ∏_{i=0}^{k-1} (n-1-i), starts at 1 */

    seq_t three_n = (seq_t)n * (seq_t)3;   /* 3*n as seq_t */

    for (idx_t k = 0; k < n; ++k) {
        /* add current term to the sum */
        sum += prod * binom;

        /* Prepare binomial and product for the next k (if any) */
        if (k + 1 < n) {
            /* C(3n, k+1) = C(3n, k) * (3n - k) / (k+1) */
            seq_t numer = binom * (three_n - (seq_t)k);
            seq_t denom = (seq_t)(k + 1);
            binom = numer / denom;            /* exact division */

            /* product ∏_{i=0}^{k} (n-1-i) for the next step */
            prod = prod * ((seq_t)(n - 1 - k));
        }
    }

    return sum;
}
