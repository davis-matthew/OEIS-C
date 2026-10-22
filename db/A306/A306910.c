/*
 * A306910  Expansion of Sum_{k>=0} x^(k^2) * Product_{j=1..k} (1 + j*x^j).
 * https://oeis.org/A306910
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   * a(n) = coefficient of x^n in Σ_{k≥0} x^(k^2) * Π_{j=1..k} (1 +
 *            j*x^j) * (OEIS A306910) * * The implementation builds the
 *            required polynomials P_k(x) = Π_{j=1..k} (1 + j*x^j) * and
 *            accumulates their shifted contributions to the sequence. *
 *            Complexity: O(n*sqrt(n)) time, O(n) space. * The routine...
 * Range:     u8 n <= 49, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   alloc_seq, free_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>

/* Helper allocation functions from the utilities framework. */
#include "../utils/alloc_seq.h"
#include "../utils/free_seq.h"

seq_t a(idx_t n)
{
    /* Allocate working arrays up to index n (inclusive). */
    size_t len = (size_t)n + 1ull;
    seq_t *A = alloc_seq(len);   /* stores final sequence coefficients */
    seq_t *P = alloc_seq(len);   /* polynomial P_k coefficients */

    /* Ensure memory is zeroed.  alloc_seq may not initialise memory. */
    memset(A, 0, len * sizeof(seq_t));
    memset(P, 0, len * sizeof(seq_t));

    /* Base case k = 0: term = 1, contributes to a(0). */
    P[0] = 1;
    A[0] += 1;

    /* Iterate over k >= 1 such that k^2 <= n. */
    for (idx_t k = 1; k * k <= n; ++k) {
        idx_t shift = k * k;                /* exponent added by k^2 */
        idx_t maxdeg = n - shift;           /* maximum degree needed in P_k */

        /* Update P to include factor (1 + k*x^k).
           Use reverse iteration to avoid contamination of source values. */
        for (idx_t d = maxdeg; d >= k; --d) {
            if (P[d - k]) {
                P[d] += (seq_t)k * P[d - k];
            }
        }

        /* Add the shifted polynomial to the final sequence array. */
        for (idx_t i = 0; i <= maxdeg; ++i) {
            if (P[i]) {
                A[i + shift] += P[i];
            }
        }
    }

    seq_t result = A[n];
    free_seq(A);
    free_seq(P);
    return result;
}
