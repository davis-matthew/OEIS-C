/*
 * A327766  Number of compositions (ordered partitions) of n into divisors of
 *          n that are at most sqrt(n).
 * https://oeis.org/A327766
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the number of ordered partitions (compositions) of n
 *            into divisors of n that are at most sqrt(n). This is computed by
 *            dynamic programming: for a fixed n let D(n) = { d | d divides n
 *            and d ≤ sqrt(n) }. Then a(0) = 1 and, for 1 ≤ m ≤ n, a(m) =
 *            Σ_{d∈D(n)} a(m-d) (with the convention a(k)=0 f...
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 35, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Integer square root for 64‑bit unsigned values; fits in O(log n). */
static inline idx_t isqrt_u64(idx_t n)
{
    idx_t res = 0;
    idx_t bit = ((idx_t)1) << 62;   /* largest power of 4 ≤ 2^64 */
    while (bit > n) bit >>= 2;
    while (bit) {
        idx_t tmp = res + bit;
        if (n >= tmp) {
            n -= tmp;
            res = (res >> 1) + bit;
        } else {
            res >>= 1;
        }
        bit >>= 2;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    /* Compute the floor of sqrt(n) */
    const idx_t sqrt_n = isqrt_u64(n);

    /* Build list of divisors of n that are ≤ sqrt(n) */
    /* The number of such divisors is at most sqrt(n)+1 which is small for the intended limits. */
    idx_t *divs = (idx_t*)malloc((size_t)(sqrt_n + 1) * sizeof(idx_t));
    if (!divs) return (seq_t)0;          /* allocation failure → error value */
    size_t dcnt = 0;
    for (idx_t d = 1; d <= sqrt_n; ++d) {
        if (n % d == 0) {
            divs[dcnt++] = d;
        }
    }

    /* Dynamic programming array */
    seq_t *dp = (seq_t*)calloc((size_t)(n + 1), sizeof(seq_t));
    if (!dp) {
        free(divs);
        return (seq_t)0;
    }
    dp[0] = (seq_t)1;

    for (idx_t m = 1; m <= n; ++m) {
        seq_t sum = 0;
        for (size_t i = 0; i < dcnt; ++i) {
            idx_t d = divs[i];
            if (m < d) continue;
            sum += dp[(size_t)(m - d)];
        }
        dp[(size_t)m] = sum;
    }

    seq_t result = dp[(size_t)n];
    free(dp);
    free(divs);
    return result;
}
