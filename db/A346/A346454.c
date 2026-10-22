/*
 * A346454  a(n) = [x^n] Product_{k>=1} (1 + n^(k+1)*x^k) / (1 - n^(k+1)*x^k).
 * https://oeis.org/A346454
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [x^n] ∏_{k≥1} (1 + n^{k+1}x^k) / (1 - n^{k+1}x^k) (offset
 *            0)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 14,
 *            big any n
 * Helpers:   seq_alloc_new_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_free_array.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;   /* a(0) = 1 */

    size_t N = (size_t)n;

    /* Allocate DP arrays and powers of n */
    seq_t *dp   = seq_alloc_new_array(N + 1);
    seq_t *dp2  = seq_alloc_new_array(N + 1);
    seq_t *pow_n = seq_alloc_new_array(N + 1);

    /* initialise arrays */
    for (size_t i = 0; i <= N; ++i) {
        dp[i]  = (seq_t)0;
        dp2[i] = (seq_t)0;
    }
    dp[0] = (seq_t)1;

    /* powers of n: pow_n[i] = n^i */
    pow_n[0] = (seq_t)1;
    seq_t nseq = (seq_t)n;
    for (size_t i = 1; i <= N; ++i)
        pow_n[i] = pow_n[i - 1] * nseq;

    /* Main DP: multiply successive factor series
       (1 + 2 n x^k + 2 n^2 x^{2k} + ...) for each k = 1..n   */
    for (uint64_t k = 1; k <= n; ++k) {
        /* start with copy of current dp */
        for (size_t i = 0; i <= N; ++i)
            dp2[i] = dp[i];

        uint64_t maxm = n / k;           /* maximum multiplicity of size k */
        for (uint64_t m = 1; m <= maxm; ++m) {
            seq_t coeff = pow_n[m];
            coeff = coeff * (seq_t)2;      /* coefficient = 2 * n^m */
            uint64_t e = m * k;            /* exponent contributed by this group */
            size_t limit = N - e;
            for (size_t s = 0; s <= limit; ++s) {
                if (dp[s] == 0) continue;
                dp2[s + e] += dp[s] * coeff;
            }
        }

        /* swap dp and dp2 for next iteration */
        seq_t *tmp = dp;
        dp  = dp2;
        dp2 = tmp;
    }

    /* a(n) = n^n * dp[n] */
    seq_t result = pow_n[n] * dp[n];

    /* free allocated arrays */
    seq_free_array(dp2);    /* dp2 is the old array after last swap */
    seq_free_array(dp);
    seq_free_array(pow_n);

    return result;
}
