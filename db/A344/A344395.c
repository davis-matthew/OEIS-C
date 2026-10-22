/*
 * A344395  a(n) = binomial(4*n - 1, 2*n - 1)*hypergeom([-n, -n + 1/2], [2*n +
 *          1], 4).
 * https://oeis.org/A344395
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{j=0..4*n-1} C(4*n-1, j) * (C(4*n-1-j, j+2*n-1) -
 *            C(4*n-1-j, j+2*n+1)) for n ≥ 1; a(0) = 1 The loop is truncated
 *            at j = n since for j > n the inner binomials are zero
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 17,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* Pre‑compute constants. */
    const idx_t N = 4 * n - 1;   /* upper index in the outer binomial */
    const idx_t M = 2 * n - 1;   /* the offset in the inner binomials */

    seq_t sum      = 0;
    seq_t binomNj  = 1;          /* C(N,0) */

    for (idx_t j = 0; j <= n; ++j) {
        /* U = N - j  and  K1 = j + M = j + 2*n - 1 */
        const idx_t U  = N - j;
        const idx_t K1 = j + M;
        const idx_t K2 = K1 + 2;      /* j + 2*n + 1 */

        seq_t term1 = 0;
        seq_t term2 = 0;

        if (K1 <= U)
            term1 = binomial(U, K1);

        if (K2 <= U)
            term2 = binomial(U, K2);

        const seq_t diff = term1 - term2;   /* non‑negative */
        sum += binomNj * diff;

        /* Update C(N, j+1) for the next iteration. */
        if (j < n) {
            const seq_t num = binomNj * (seq_t)(N - j);
            binomNj = num / (seq_t)(j + 1);
        }
    }

    return sum;
}
