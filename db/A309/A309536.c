/*
 * A309536  Total number of triangular numbers in all compositions of n.
 * https://oeis.org/A309536
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k≥1, k(k+1)/2 ≤ n} b(n - k(k+1)/2) * where b(m) is the
 *            coefficient of x^m in (1 - x)^2 / (1 - 2x)^2. * Computed
 *            directly by evaluating the closed form of b(m)
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 29, u64 n <= 60, u128 n <= 123,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#ifdef __cplusplus
#include <vector>
#endif

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

#ifdef __cplusplus
    std::vector<seq_t> pow2(n + 1);
    std::vector<seq_t> b(n + 1);
#else
    seq_t *pow2 = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    seq_t *b = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!pow2 || !b)
        return 0; /* memory allocation failed */
#endif

    /* power of two array */
    pow2[0] = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i)
        pow2[i] = pow2[i - 1] * (seq_t)2;

    /* b[0] = 1, b[1] = 2 */
    b[0] = (seq_t)1;
    if (n >= 1)
        b[1] = (seq_t)2;

    /* compute b[i] for i ≥ 2: b(i) = (i+1)*2^i - 2*i*2^(i-1) + (i-1)*2^(i-2) */
    for (idx_t i = 2; i <= n; ++i) {
        seq_t term1 = ((seq_t)(i + 1)) * pow2[i];
        seq_t term2 = ((seq_t)i) * pow2[i - 1];
        seq_t term3 = ((seq_t)(i - 1)) * pow2[i - 2];
        b[i] = term1 - ((seq_t)2 * term2) + term3;
    }

    /* accumulate contributions of all triangular parts */
    seq_t ans = (seq_t)0;
    for (idx_t k = 1; ; ++k) {
        seq_t tri = ((seq_t)k * (seq_t)(k + 1)) / (seq_t)2;
        if (tri > (seq_t)n)
            break;
        ans += b[n - (idx_t)tri];
    }

#ifndef __cplusplus
    free(pow2);
    free(b);
#endif
    return ans;
}
