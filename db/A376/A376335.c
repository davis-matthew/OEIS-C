/*
 * A376335  Irregular triangle read by rows: T(n,k) = A008949(n-1,k) if 0 <= k
 *          <= n - 2 otherwise A008949(n-1,2*n-4-k) if n - 2 <= k <= 2*n - 4.
 * https://oeis.org/A376335
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy, tabf
 * Formula:   not stated
 * Range:     u8 n <= 69, u16 n <= 82, u32 n <= 82, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS sequence A376335
 *
 * The sequence is obtained by concatenating the rows of the
 * irregular triangle T(N,K):
 *
 *   T(N,K) = Σ_{i=0}^{min(K,N-2)} C(N-1,i)
 *            (and T(N,K)=T(N,2*N-4-K) for K>N-2)
 *
 * Rows are indexed by N≥2; the first row has length 1,
 * the second has length 3, the third has 5, … and the
 * total number of elements up to row N equals (N‑1)².
 * The overall sequence a(n) (with offset 2) is the
 * concatenation of these rows.
 *
 * This implementation computes a(n) directly without
 * building the entire triangle.
 */

#include <stdint.h>

/* ---------- integer square‑root for idx_t ---------- */
static idx_t _isqrt_u64(idx_t x)
{
    idx_t lo = 0, hi = x, ans = 0;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq <= x) {
            ans = mid;
            lo  = mid + 1;
        } else {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }
    return ans;
}

/* ---------- main sequence function ---------- */
seq_t a(idx_t n)
{
    if (n < 2)           /* below offset: undefined */
        return (seq_t)0;

    /* 0‑based position in the concatenated sequence */
    idx_t idx0 = n - 2;

    /* Row number N (≥2) containing this element */
    idx_t root = _isqrt_u64(idx0);   /* floor(sqrt(idx0))   */
    idx_t N    = root + 2;           /* N = floor_sqrt(idx0)+2 */

    /* Number of terms before this row: (N-2)²   */
    idx_t prev_total = (N - 2) * (N - 2);

    /* Offset within the row (0‑based) */
    idx_t offset = idx0 - prev_total;          /* 0 ≤ offset < 2*N-3 */
    idx_t k = offset;                          /* column index */

    /* For the triangle definition let m = N-1  */
    idx_t m = N - 1;                           /* 1 ≤ m                */

    /* Reduce k to the first half by symmetry */
    idx_t k_small;
    if (k <= m - 1)
        k_small = k;
    else
        k_small = (2 * m - 2) - k;             /* mirror index   */

    /* Compute Σ_{i=0}^{k_small} C(m,i) */
    seq_t sum   = 1;           /* binom(m,0)   */
    seq_t binom = 1;           /* current binom(m,i)   */
    for (idx_t i = 1; i <= k_small; ++i) {
        binom = binom * (seq_t)(m - i + 1) / (seq_t)i;
        sum += binom;
    }
    return sum;
}
