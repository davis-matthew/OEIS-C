/*
 * A242656  Mahonian numbers T(n,6) (cf. A008302).
 * https://oeis.org/A242656
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = T(n,6), the Mahonian number: number of permutations of n
 *            elements with exactly 6 inversions. OEIS A242656, offset 4
 * Range:     u8 n <= 6, u16 n <= 17, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#define K 6                 /* fixed inversion count */
#define K1 (K + 1)          /* array length */

seq_t a(idx_t n)
{
    if (n < 4)
        return 0;         /* undefined for n < offset */

    /* dp[k] holds T(m, k) after processing m elements */
    seq_t dp[K1];
    seq_t next[K1];

    /* initial state: T(0,0)=1, others 0 */
    dp[0] = 1;
    for (int j = 1; j < K1; ++j)
        dp[j] = 0;

    for (idx_t m = 1; m <= n; ++m) {
        for (int k = 0; k < K1; ++k) {
            next[k] = 0;
            /* the contribution of inserting the m‑th element may bring
               at most min(k, m-1) inversions. */
            int max_i = (k < (int)m) ? k : (int)(m - 1);
            for (int i = 0; i <= max_i; ++i)
                next[k] += dp[k - i];
        }
        /* copy next to dp for the next iteration */
        for (int k = 0; k < K1; ++k)
            dp[k] = next[k];
    }

    return dp[K];
}
