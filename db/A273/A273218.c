/*
 * A273218  G.f. A(x) satisfies: A(x) = A(x^2 - x^3)/x.
 * https://oeis.org/A273218
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = sum_{k = ceil((n+1)/3)}^{floor((n+1)/2)} a(k) *
 *            (-1)^{n+1-2k} * C(k, n+1-2k) Initial value: a(1) = 1 (C(k,j) is
 *            the binomial coefficient)
 * Range:     i8 n <= 16, i16 n <= 31, i32 n <= 60, i64 n <= 115,
 *            i128 n <= 225, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *A = NULL;          /* array of coefficients, 1‑based */
static idx_t A_len = 0;          /* highest index currently computed */

/* recompute all coefficients up to limit (inclusive) */
static void compute_up_to(idx_t limit)
{
    if (limit == 0) return;
    if (A) free(A);
    A = (seq_t *)calloc((size_t)(limit + 1), sizeof(seq_t));
    if (!A) return;          /* allocation failure → undefined behaviour */

    A[1] = (seq_t)1;          /* a(1) = 1 */
    idx_t k_max = (limit + 1) / 2;     /* largest k that can influence a[n] ≤ limit */

    for (idx_t k = 1; k <= k_max; ++k) {
        idx_t start_n = 2 * k - 1;
        if (start_n > limit) continue;

        /* j ranges from 0 to min(k, limit - start_n) */
        idx_t j_max = (limit - start_n < k) ? (limit - start_n) : k;

        seq_t binom = (seq_t)1;     /* C(k,0) */
        int sign = 1;               /* (-1)^j */

        for (idx_t j = 0; j <= j_max; ++j) {
            idx_t n_idx = start_n + j;
            if (n_idx != 1) {       /* a(1) is already fixed */
                seq_t tmp = A[k] * binom;
                if (sign < 0) tmp = -tmp;
                A[n_idx] += tmp;
            }
            if (j == j_max) break;   /* avoid updating when not needed */

            /* update binomial coefficient for next j:
               C(k,j+1) = C(k,j) * (k-j) / (j+1)   */
            seq_t mult = (seq_t)(k - j);
            seq_t divi = (seq_t)(j + 1);
            binom = (binom * mult) / divi;
            sign = -sign;
        }
    }
    A_len = limit;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (!A || n > A_len) compute_up_to(n);
    return A[n];
}
