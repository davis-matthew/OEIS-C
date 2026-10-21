/*
 * A254881  Triangle read by rows, T(n,k) = sum(j=0..k-1, S(n+1,j+1)*S(n,k-j))
 *          where S denotes the Stirling cycle numbers A132393, T(0,0)=1,
 *          n>=0, 0<=k<=2n.
 * https://oeis.org/A254881
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = T(floor(sqrt(n)), n - floor(sqrt(n))^2) where T(r,k) =
 *            Σ_{j=0}^{k-1} S(r+1,j+1)*S(r,k-j) and S(m,l) are unsigned
 *            Stirling cycle numbers of the first kind
 * Range:     u8 n <= 17, u16 n <= 36, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* maximum Stirling row needed for n up to 10000 (r <= 100)
   using a safe upper bound */
#define MAX_S 2000

/* table of Stirling cycle numbers */
static seq_t S[MAX_S + 1][MAX_S + 1];
static idx_t S_max = 0;

/* integer square‑root (floor) for idx_t */
static inline idx_t isqrt_u64(idx_t n)
{
    idx_t low  = 0, high = ((idx_t)1 << 32);   /* 2^32 > sqrt(UINT64_MAX) */
    idx_t ans  = 0;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq <= n) {
            ans  = mid;
            low  = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return ans;
}

/* ensure Stirling table up to max_n (inclusive) */
static void ensure_S(idx_t max_n)
{
    if (S_max >= max_n) return;                 /* already done */

    if (S_max == 0) {                           /* initialize base row */
        S[0][0] = (seq_t)1;
        /* other entries in row 0 are already zero */
    }

    for (idx_t n = S_max + 1; n <= max_n; ++n) {
        S[n][0] = (seq_t)0;
        for (idx_t k = 1; k <= max_n; ++k) {
            seq_t t1 = S[n - 1][k - 1];
            seq_t t2 = (seq_t)(n - 1) * S[n - 1][k];
            S[n][k] = t1 + t2;
        }
    }
    S_max = max_n;
}

/* triangle entry T(n,k) for n >= 0, 0 <= k <= 2n */
static seq_t T(idx_t n, idx_t k)
{
    if (n == 0)                     /* only k == 0 exists */
        return k == 0 ? (seq_t)1 : (seq_t)0;

    if (k == 0)                     /* row terminus for n > 0 */
        return (seq_t)0;

    /* need Stirling numbers up to row n+1 */
    ensure_S(n + 1);

    /* valid j range: max(0, k - n) <= j <= min(k - 1, n) */
    idx_t j_start = (k > n) ? (k - n) : 0;
    idx_t j_end   = (k - 1 < n) ? (k - 1) : n;

    seq_t sum = (seq_t)0;
    for (idx_t j = j_start; j <= j_end; ++j) {
        seq_t a = S[n + 1][j + 1];
        seq_t b = S[n][k - j];
        sum += a * b;
    }
    return sum;
}

/* public function as required */
seq_t a(idx_t n)
{
    idx_t r = isqrt_u64(n);            /* floor of sqrt(n) */
    idx_t k = n - r * r;
    return T(r, k);
}
