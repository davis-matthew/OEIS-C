/*
 * A248053  G.f.: Sum_{n>=0} x^n / (1-4*x)^(2*n+1) * [Sum_{k=0..n} C(n,k)^2 *
 *          2^k * x^k] * [Sum_{k=0..n} C(n,k)^2 * 4^k * x^k].
 * https://oeis.org/A248053
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..⌊n/2⌋} 2^k * * Sum_{j=0..n-2k} C(n-k, k+j)^2 *
 *            C(k+j, j)^2 * 4^j
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 21,
 *            big any n
 * Helpers:   mul_small, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    const idx_t max_k = n >> 1;          /* floor(n/2) */
    seq_t result = 0;

    for (idx_t k = 0; k <= max_k; ++k)
    {
        idx_t m = n - (k << 1);          /* n - 2k */

        /* compute A = C(n-k, k) iteratively */
        seq_t A = 1;
        for (idx_t t = 1; t <= k; ++t)
            A = (A * (seq_t)(n - 2 * k + t)) / (seq_t)t; /* C(n-k, k) */

        seq_t B = 1;                     /* C(k, 0) */
        seq_t pow4 = 1;
        seq_t inner = 0;

        for (idx_t j = 0; j <= m; ++j)
        {
            seq_t term = A * A;          /* A^2 */
            term *= B * B;              /* *B^2 */
            term *= pow4;               /* *4^j */
            inner += term;

            if (j == m) break;          /* last iteration */

            /* Update A: C(n-k, k+j+1) from C(n-k, k+j) */
            seq_t numA = (seq_t)(n - 2 * k - j);
            seq_t denA = (seq_t)(k + j + 1);
            A = (A * numA) / denA;

            /* Update B: C(k+j+1, j+1) from C(k+j, j) */
            seq_t numB = (seq_t)(k + j + 1);
            seq_t denB = (seq_t)(j + 1);
            B = (B * numB) / denB;

            /* Update power of 4 */
            pow4 = mul_small(pow4, 4);
        }

        seq_t pk = pow2(k);               /* 2^k */
        result += inner * pk;
    }

    return result;
}
