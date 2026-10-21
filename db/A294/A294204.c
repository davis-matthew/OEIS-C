/*
 * A294204  Number of partitions of n into distinct Lucas parts (A000032)
 *          greater than 1.
 * https://oeis.org/A294204
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into distinct Lucas parts
 *            greater than 1. Lucas numbers L(0)=2, L(1)=1,
 *            L(k)=L(k−1)+L(k−2). The allowed parts are L(0)=2 and L(k) for
 *            k≥2 (i.e. 3,4,7,11,18,...)
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Gather allowed Lucas numbers up to n. */
    idx_t parts[32];
    size_t pcnt = 0;
    seq_t a0 = 2, a1 = 1;                     /* L0, L1 */
    if (a0 <= n) parts[pcnt++] = (idx_t)a0;   /* include L0=2 */

    while (1) {
        seq_t c = a0 + a1;                    /* next Lucas number L(k) */
        if (c > n) break;
        a0 = a1; a1 = c;                      /* shift: L(k-1), L(k) */
        if (c > 1) parts[pcnt++] = (idx_t)c;  /* include all L(k) (k≥2) */
    }

    /* DP array for subset-sum counts (distinct parts). */
    seq_t *dp = (seq_t*)calloc((size_t)n + 1, sizeof(seq_t));
    if (dp == NULL) return 0;  /* in case of allocation failure */
    dp[0] = 1;                  /* empty partition */

    for (size_t i = 0; i < pcnt; ++i) {
        idx_t part = parts[i];
        for (idx_t s = n; s >= part; --s) {
            dp[s] += dp[s - part];
        }
    }

    seq_t res = dp[n];
    free(dp);
    return res;
}
