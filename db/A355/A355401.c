/*
 * A355401  Triangle read by rows: T(n, k) = Sum_{i=1..n-k}
 *          inverse-q-binomial(n-k-1, i-1) * q-binomial(n-2+i, n-2) for 0 < k
 *          < n with initial values T(n, 0) = 0 for n > 0 and T(n, n) = 1 for
 *          n >= 0, here q = 2.
 * https://oeis.org/A355401
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) is obtained by reading the rows of the triangle T(n,k)
 *            defined by T(0,0) = 1 T(n,0) = 0 for n>0 T(n,n) = 1 T(n,k) =
 *            2^((n-k-1)*(n-k)) * [n-1,k-1]_2 for 0<k<n where [n,k]_2 is the
 *            q‑binomial coefficient with q = 2. The sequence a(n) lists the
 *            entries of T row by row, i.e. a(0) = T(0,0), a(1)=...
 * Range:     u8 n <= 15, u16 n <= 21, u32 n <= 36, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"       /* seq_t pow2(idx_t) */

/* computes the q-binomial coefficient [N,K]_2 for 0<=K<=N */
static seq_t binom_q2(idx_t N, idx_t K)
{
    seq_t result = 1;
    /* iterate K times: multiply by (2^(N-K+i)-1)/(2^i-1) */
    for (idx_t i = 1; i <= K; ++i)
    {
        seq_t numer = pow2(N - K + i) - 1;
        seq_t denom = pow2(i) - 1;
        result = (result * numer) / denom;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* Special case n == 0 */
    if (n == 0)
        return 1;

    /* Find the row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;

    /* Column index within the row */
    idx_t c = n - r * (r + 1) / 2;

    /* Corner cases */
    if (c == 0)
        return 0;          /* T(n,0) for n>0 */
    if (c == r)
        return 1;          /* T(n,n) */

    /* General case: T(r,c) with r>0 and 0<c<r */
    idx_t N = r - 1;
    idx_t K = c - 1;

    seq_t binomial = binom_q2(N, K);
    /* exponent for the power of 2 */
    idx_t diff = r - c;                /* r - c >= 1 */
    idx_t exp  = (diff - 1) * diff;    /* (r-c-1)*(r-c) */
    seq_t pow   = pow2(exp);

    return binomial * pow;
}
