/*
 * A370505  T(n,k) is the difference between the number of k-dist-increasing
 *          and (k-1)-dist-increasing permutations of [n], where p is
 *          k-dist-increasing if k>=0 and p(i)<p(i+k) for all i in [n-k];
 *          triangle T(n,k), n>=0, 0<=k<=n, read by rows.
 * https://oeis.org/A370505
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 52, u32 n <= 115, u64 n <= 251,
 *            u128 n <= 661, big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A370505: flattening of the triangle T(n,k) where
 * T(n,k) = A248686(n,k) - A248686(n,k-1).
 * A248686(n,k) is the number of k‑dist‑increasing permutations of [n],
 *   = n! / ((q)!^(k-m) * (q+1)!^m) with q = ⌊n/k⌋, m = n mod k.
 * For k = 0 the value is 1 if n = 0, otherwise 0.
 *
 * The flattened index n is mapped to row r and column k by
 *    r = ⌊(√(8n+1)-1)/2⌋,
 *    k = n - r(r+1)/2.  Here 0 ≤ k ≤ r.
 *
 * The implementation uses only integer arithmetic and the provided
 * `seq_t binom(idx_t a, idx_t b)` routine.
 */

#include "../utils/binom.h"   /* provides seq_t binom(idx_t a, idx_t b) */

static /* compute A248686(n,k) for k≥1 */
seq_t multinomial(idx_t n, idx_t k)
{
    /* n ≥ k ≥ 1 */
    idx_t q = n / k;     /* floor(n/k) */
    idx_t m = n % k;     /* residue */

    seq_t result = 1;
    idx_t pref = 0;

    for (idx_t j = 1; j <= k; ++j) {
        idx_t len = (j <= m) ? q + 1 : q;
        pref += len;
        if (j > 1) {
            result *= binom(pref, len);
        }
    }
    return result;
}

/* find the row r such that r(r+1)/2 ≤ n < (r+1)(r+2)/2   */
static idx_t row_of_idx(idx_t n)
{
    idx_t low = 0;
    idx_t high = 1;
    /* find an upper bound                */
    while ((unsigned __int128)high * (high + 1) / 2 <= n) {
        if (high > (idx_t)-2)
            break;
        high <<= 1;
    }
    /* binary search between low and high */
    while (low < high) {
        idx_t mid = low + (high - low + 1) / 2;
        if ((unsigned __int128)mid * (mid + 1) / 2 <= n)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    idx_t r = row_of_idx(n);                     /* row */
    idx_t k = n - r * (r + 1) / 2;               /* column */

    /* Handle the special 0‑dist‑increasing case */
    if (k == 0) {
        return (r == 0) ? (seq_t)1 : (seq_t)0;
    }

    seq_t Bk  = multinomial(r, k);             /* A248686(r,k)   */
    seq_t Bk1;
    if (k == 1) {                              /* A248686(r,0) = 0 for r>0 */
        Bk1 = (seq_t)0;
    } else {
        Bk1 = multinomial(r, k - 1);
    }
    return Bk - Bk1;
}
