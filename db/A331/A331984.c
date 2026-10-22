/*
 * A331984  Number of compositions (ordered partitions) of n into distinct
 *          square pyramidal numbers.
 * https://oeis.org/A331984
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*{
  Formula: a(n) = Σ_{S⊆P, ΣS=n} |S|!
  where P = { square pyramidal numbers ≤ n }, i.e. P(k)=k(k+1)(2k+1)/6.
  The function counts ordered compositions of n into distinct square pyramidal
  numbers by first counting subsets of each size that sum to n, then
  weighting each subset by the factorial of its size.
}*/
#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Generate all square pyramidal numbers ≤ n.  P(k) = k(k+1)(2k+1)/6. */
    idx_t vals[64];                      /* enough for n up to >10⁹ */
    size_t m = 0;                        /* number of values found */
    for (idx_t k = 1; ; ++k) {
        idx_t P = k * (k + 1) * (2 * k + 1) / 6;
        if (P > n) break;
        vals[m++] = P;
    }

    /* DP array: dp[s*(m+1)+c] = number of subsets of size c summing to s. */
    size_t len   = (size_t)(n + 1) * (m + 1);
    seq_t *dp    = (seq_t *)malloc(sizeof(seq_t) * len);
    if (!dp) return 0; /* memory failure – return 0 as fallback */
    for (size_t i = 0; i < len; ++i) dp[i] = 0;
    dp[0] = 1;                                /* dp[0][0] = 1 */

    for (size_t i = 0; i < m; ++i) {
        idx_t val = vals[i];
        for (idx_t s = n; s >= val; --s) {
            for (size_t c = m; c >= 1; --c) {
                dp[s * (m + 1) + c] += dp[(s - val) * (m + 1) + c - 1];
            }
        }
    }

    /* Pre‑compute factorials up to m. */
    seq_t fact[64];
    fact[0] = 1;
    for (size_t i = 1; i <= m; ++i) fact[i] = fact[i - 1] * (seq_t)i;

    /* Sum over all subset sizes with factorial weight. */
    seq_t result = 0;
    for (size_t c = 0; c <= m; ++c) {
        result += dp[n * (m + 1) + c] * fact[c];
    }

    free(dp);
    return result;
}
