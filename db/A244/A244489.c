/*
 * A244489  Triangle read by rows: T(n,k) = Sum_{j=k..n}
 *          binomial(n,j)*Stirling_2(j,k)*Bell(n-j), where Bell(n) =
 *          A000110(n), for n >= 1, 0 <= k <= n-1.
 * https://oeis.org/A244489
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(n,k) = Σ_{j=k..n} C(n,j) * S₂(j,k) * B(n-j)
 * Range:     u8 n <= 16, u16 n <= 37, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* T(n,k) is the (k+1)‑th entry in the n‑th row (0 ≤ k < n). */
/* The sequence a(m) is the triangle read by rows, i.e. a(m)=T(r,k) */
/* where r and k are obtained from the flattened index m (m ≥ 1). */

#include <stdlib.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    /* Find row r such that  r(r‑1)/2 < n ≤ r(r+1)/2  */
    idx_t r = 0;
    idx_t cum = 0;
    while (cum < n) {
        ++r;
        cum += r;
    }

    /* Position inside the row: 1‑based, k = pos‑1  */
    idx_t prev_cum = cum - r;
    idx_t pos = n - prev_cum;
    idx_t k = pos - 1;                /* 0 ≤ k ≤ r-1 */

    /* Allocate array for Stirling numbers S₂(i,j) (0 ≤ i ≤ r, 0 ≤ j ≤ i) */
    size_t sz = (size_t)(r + 1);
    seq_t *S2 = (seq_t *)calloc(sz * sz, sizeof(seq_t));
    if (!S2) return 0;
    S2[0] = 1;                         /* S₂(0,0) = 1 */

    for (idx_t i = 1; i <= r; ++i) {
        for (idx_t j = 1; j <= i; ++j) {
            seq_t term1 = S2[(size_t)(i - 1) * sz + j];
            term1 *= (seq_t)j;
            seq_t term2 = S2[(size_t)(i - 1) * sz + j - 1];
            S2[(size_t)i * sz + j] = term1 + term2;
        }
    }

    /* Allocate array for Bell numbers  B(0..r)  */
    seq_t *Bell = (seq_t *)calloc(sz, sizeof(seq_t));
    if (!Bell) {
        free(S2);
        return 0;
    }
    Bell[0] = 1;                       /* B₀ = 1 */
    for (idx_t m = 1; m <= r; ++m) {
        seq_t sum = 0;
        for (idx_t j = 0; j < m; ++j) {
            sum += binomial(m - 1, j) * Bell[j];
        }
        Bell[m] = sum;
    }

    /* Compute T(r,k) */
    seq_t result = 0;
    for (idx_t j = k; j <= r; ++j) {
        seq_t term = binomial(r, j);
        term *= S2[(size_t)j * sz + k];
        term *= Bell[r - j];
        result += term;
    }

    free(S2);
    free(Bell);
    return result;
}
