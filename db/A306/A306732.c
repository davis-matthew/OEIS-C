/*
 * A306732  Expansion of Sum_{k>=0} x^(k*(k+1)/2) * Product_{j=1..k} (1 +
 *          j*x^j).
 * https://oeis.org/A306732
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 33, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A306732
    a(n) = coefficient of x^n in Σ_{k≥0} x^{k(k+1)/2} * ∏_{j=1..k}(1 + j*x^j)
    Computation:  Build the polynomials P_k(x)=∏_{j=1..k}(1 + j*x^j) incrementally
    and add their shifted versions to the result array.  Complexity: O(n·√n). */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Convert the index to a C type suitable for array handling */
    const size_t N = (size_t)n;

    /* Allocate arrays to hold the current polynomial P_k and the
       accumulated answer.  Use malloc and initialise all elements to 0
       so that any seq_t default constructor is invoked via the
       assignment. */
    seq_t *dp   = (seq_t*)malloc((N + 1) * sizeof(seq_t));
    seq_t *poly = (seq_t*)malloc((N + 1) * sizeof(seq_t));
    if (!dp || !poly) { abort(); }
    for (size_t i = 0; i <= N; ++i) { dp[i] = 0; poly[i] = 0; }

    /* k = 0 contributes 1 at x^0 */
    dp[0]   = 1;
    poly[0] = 1;

    /* Find the largest k with k*(k+1)/2 <= n */
    idx_t kmax = 0;
    while (1) {
        seq_t next = (seq_t)(kmax + 1) * ((seq_t)(kmax + 2)) / 2;
        if (next > n) break;
        ++kmax;
    }

    /* Iterate over k = 1 .. kmax */
    for (idx_t k = 1; k <= kmax; ++k) {
        /* Update the polynomial: P_k(x) = P_{k-1}(x) * (1 + k*x^k) */
        for (idx_t i = N; i >= k; --i) {
            poly[i] += (seq_t)k * poly[i - k];
        }

        /* Add the shifted polynomial to dp: shift = k*(k+1)/2 */
        seq_t shift = (seq_t)k * ((seq_t)k + 1) / 2;
        if (shift <= (seq_t)N) {
            const size_t s = (size_t)shift;
            for (size_t j = s; j <= N; ++j) {
                dp[j] += poly[j - s];
            }
        }
    }

    seq_t result = dp[n];
    free(dp);
    free(poly);
    return result;
}
