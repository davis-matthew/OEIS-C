/*
 * A300307  Number of solutions to 1 +- 3 +- 6 +- ... +- n*(n+1)/2 == 0 mod n.
 * https://oeis.org/A300307
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   number of sign assignments to triangular numbers: 1 ± 3 ± 6 ± …
 *            ± n(n+1)/2 ≡ 0 (mod n)
 * Range:     u8 n <= 11, u16 n <= 21, u32 n <= 37, u64 n <= 71, u128 n <= 135,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* not needed for OEIS offset but safe */

    size_t sz = (size_t)n;
    /* Allocate two arrays for DP */
    seq_t *dp = (seq_t*)calloc(sz, sizeof(seq_t));
    seq_t *ndp = (seq_t*)calloc(sz, sizeof(seq_t));
    if (!dp || !ndp) return 0;     /* allocation failed */

    /* First triangular number (1) is fixed with a plus sign */
    idx_t start = 1 % n;
    dp[start] = 1;

    for (idx_t k = 2; k <= n; ++k) {
        /* compute T_k = k(k+1)/2 and its value modulo n */
        uint64_t tri = (uint64_t)k * (k + 1) / 2;
        idx_t t = (idx_t)(tri % n);

        /* reset ndp to zeros */
        for (size_t i = 0; i < sz; ++i) ndp[i] = 0;

        /* DP transition */
        for (size_t i = 0; i < sz; ++i) {
            seq_t val = dp[i];
            if (!val) continue;
            idx_t pos1 = (i + t) % n;
            ndp[pos1] += val;
            idx_t pos2 = (i + n - t) % n;
            ndp[pos2] += val;
        }

        /* swap dp and ndp for next iteration */
        seq_t *tmp = dp;
        dp = ndp;
        ndp = tmp;
    }

    seq_t result = dp[0];
    free(dp);
    free(ndp);
    return result;
}
