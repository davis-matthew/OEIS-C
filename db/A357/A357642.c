/*
 * A357642  Number of even-length integer compositions of 2n whose
 *          half-alternating sum is 0.
 * https://oeis.org/A357642
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 18, u64 n <= 34, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 0 if n = 1; a(0) = 1.
 * For n > 0:
 *   Let N = n - 1.
 *   For 1 ≤ m ≤ ⌊n/2⌋, let k = 2m - 1.
 *   Let c_k = binomial(N, k).
 *   Then a(n) = Σ_{m} c_k * (c_k + c_{k-2}), treating c_{-1} = 0.
 *   (This follows from the combinatorial derivation using the
 *    generating function of the pair sums.)
 *
 * This implementation computes the binomial coefficients
 * iteratively using C(n,k) = C(n,k-1) * (n-k+1)/k,
 * which requires only integer arithmetic.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle the special case a(0) = 1. */
    if (n == (idx_t)0)
        return (seq_t)1;

    /* For n ≥ 1. */
    idx_t N = n - 1;                /* N = n-1  (≥ 0) */
    /* Allocate array for binomial coefficients C[N+1]. */
    seq_t *C = (seq_t *)malloc((N + 1) * sizeof(seq_t));
    if (!C) return (seq_t)0;        /* failed allocation: return 0 */

    C[0] = (seq_t)1;                /* C(N,0) = 1 */
    for (idx_t k = 1; k <= N; ++k)
    {
        /* C(N,k) = C(N,k-1) * (N - k + 1) / k  */
        seq_t numer = C[k - 1] * (seq_t)(N - k + 1);
        C[k] = numer / (seq_t)k;
    }

    seq_t ans = (seq_t)0;
    idx_t m_max = n >> 1;           /* floor(n/2) */
    for (idx_t m = 1; m <= m_max; ++m)
    {
        idx_t k = (m << 1) - 1;     /* k = 2m-1, always odd and ≤ N */
        seq_t ck = C[k];
        seq_t ck_prev = (k >= 3) ? C[k - 2] : (seq_t)0; /* C[2m-3] or 0 */
        ans += ck * (ck + ck_prev);
    }

    free(C);
    return ans;
}
