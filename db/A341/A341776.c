/*
 * A341776  Number of partitions of n into 5 nonzero tetrahedral numbers.
 * https://oeis.org/A341776
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 95, u16 n <= 95, u32 n <= 95, u64 n <= 95, u128 n <= 95,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Number of partitions of n into exactly 5 non‑zero tetrahedral numbers
 *   T(k) = k(k+1)(k+2)/6.
 *   Offset: 5  (a(5) first term)
 */

#include <stdlib.h>
#ifdef __cplusplus
#include <new>
#endif

seq_t a(idx_t n)
{
    /* the sequence is defined only for n ≥ 5 */
    if (n < (idx_t)5)
        return (seq_t)0;

    const size_t M      = (size_t)(n + 1);     /* number of possible sums 0..n */
    const size_t dp_size = 6 * M;              /* 0..5 parts × sums */

    /* allocate a 1‑dimensional array that represents a 2‑D dp[6][M] */
#ifdef __cplusplus
    seq_t *dp = new seq_t[dp_size];
#else
    seq_t *dp = (seq_t *)calloc(dp_size, sizeof(seq_t));
#endif
    if (!dp)
        return (seq_t)0;
    dp[0] = (seq_t)1;         /* dp[0][0] = 1, other entries are already 0 */

    /* iterate over all tetrahedral numbers ≤ n */
    for (idx_t k = 1; ; ++k) {
        seq_t t = (seq_t)k * (seq_t)(k + 1) * (seq_t)(k + 2) / (seq_t)6;
        if (t > (seq_t)n)
            break;

        /* Update dp for partitions that use 1…5 tetrahedra */
        for (idx_t part = 1; part <= 5; ++part) {
            for (idx_t s = (idx_t)t; s <= n; ++s) {
                /* indices in the flattened array */
                size_t from = (size_t)(part - 1) * M + (s - (idx_t)t);
                size_t to   = (size_t)part * M + s;
                dp[to] += dp[from];
            }
        }
    }

    seq_t result = dp[5 * M + n];
#ifdef __cplusplus
    delete[] dp;
#else
    free(dp);
#endif
    return result;
}
