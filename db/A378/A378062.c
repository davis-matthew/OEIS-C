/*
 * A378062  Array read by ascending antidiagonals: A(n, k) = (n +
 *          1)*binomial(2*k + n - 1, k - 1)^2 / (2*k + n - 1) for k > 0, and
 *          A(n, 0) = 0.
 * https://oeis.org/A378062
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 25, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Compute A(n,k) from the OEIS definition and read its values
    along ascending antidiagonals.  For index i in the 1‑D
    sequence a(i) we find the coordinates (n_row,k) of the
    cell in the infinite tableau A:
          n_row = s - k
          k     = i - s(s+1)/2
    where s satisfies s(s+1)/2 ≤ i < (s+1)(s+2)/2.
    The table entry is
          A(n,k) = 0                     if k == 0
          A(n,k) = (n+1)*C(2k+n-1, k-1)^2 / (2k+n-1)   otherwise.
    The function returns a(i) for any i ≥ 0.  */
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Find the antidiagonal index s such that
       s*(s+1)/2 ≤ n < (s+1)*(s+2)/2. */
    idx_t s = 0;
    idx_t low = 0, high = n;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        __int128 tri = (__int128)mid * (mid + 1) / 2;
        if (tri <= n) {
            s = mid;
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }

    /* Position inside the diagonal. */
    __int128 tri_s = (__int128)s * (s + 1) / 2;
    idx_t k = n - (idx_t)tri_s;          /* 0 ≤ k ≤ s */
    idx_t n_row = s - k;                 /* n in A(n_row, k) */

    if (k == 0)                          /* A(n,0) = 0 */
        return (seq_t)0;

    /* Compute A(n_row, k) from the given formula. */
    idx_t m = 2 * k + n_row - 1;          /* m = 2k + n_row - 1 */

    seq_t bin = binomial((idx_t)m, (idx_t)(k - 1));
    seq_t val = bin * bin;                /* bin² */
    val = val * ((seq_t)n_row + 1);       /* (n_row+1)*bin² */
    val = val / (seq_t)m;                 /* divide by m */

    return val;
}
