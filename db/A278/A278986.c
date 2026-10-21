/*
 * A278986  Array read by antidiagonals downwards: T(b,n) = number of words of
 *          length n over an alphabet of size b that are in standard order and
 *          which have the property that at least one letter is repeated.
 * https://oeis.org/A278986
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = Σ_{j=1..b} S(len,j) – (len ≤ b ? 1 : 0), where (b,len)
 *            are obtained by writing n in antidiagonal order of the
 *            triangular array T(b,len)
 * Range:     u8 n <= 38, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    /* Find the diagonal S such that (S-1)S/2 ≥ n. */
    idx_t S = 2;
    idx_t tri = 1;              /* S*(S-1)/2 for current S */
    while (tri < n) {
        ++S;
        tri = (S * (S - 1)) / 2;
    }
    /* Number of terms before this diagonal. */
    idx_t prev_tri = ( (S - 1) * (S - 2) ) / 2;
    /* Position inside the diagonal (0‑based). */
    idx_t offset = n - prev_tri - 1;
    /* Convert to (b,len). */
    idx_t b   = offset + 1;     /* 1‑based letter count */
    idx_t len = S - b;          /* 1‑based word length */

    /* Allocate DP array for Stirling numbers S(len,j), j = 0..b. */
    seq_t *dp = (seq_t *)calloc(b + 1, sizeof(seq_t));
    if (!dp) return 0;          /* minimal fail‑safe */
    dp[0] = (seq_t)1;           /* S(0,0) = 1 */

    for (idx_t m = 1; m <= len; ++m) {
        idx_t maxj = (m < b ? m : b);
        for (idx_t j = maxj; j >= 1; --j) {
            /* S(m,j) = j * S(m-1,j) + S(m-1,j-1) */
            dp[j] = mul_small(dp[j], (unsigned int)j) + dp[j - 1];
        }
        dp[0] = (seq_t)0;      /* S(m,0) = 0 for m > 0 */
    }

    seq_t sum = 0;
    for (idx_t j = 1; j <= b; ++j)
        sum += dp[j];

    if (len <= b)
        sum -= (seq_t)1;       /* subtract the all‑distinct case */

    free(dp);
    return sum;
}
