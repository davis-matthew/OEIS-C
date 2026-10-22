/*
 * A328301  Expansion of Product_{k>0} 1/(1 - x^(k^k)).
 * https://oeis.org/A328301
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into parts k^k for k>0
 * Range:     u8 n <= 26, u16 n <= 2073, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    static seq_t *dp   = NULL; /* DP array: dp[i] = a(i) */
    static size_t cap   = 0;   /* allocated capacity */

    /* Extend storage if necessary.  */
    size_t need = (size_t)n + 1;
    if (need > cap) {
        seq_t *tmp = (seq_t*)realloc(dp, need * sizeof(seq_t));
        if (!tmp) abort();                 /* allocation failure is fatal */
        dp  = tmp;
        cap = need;
    }

    /* Re‑initialise up to n.  */
    for (size_t i = 0; i <= (size_t)n; ++i) dp[i] = 0;
    dp[0] = 1;

    /* Coin‑change DP over part values k^k.  */
    for (idx_t k = 1; ; ++k) {
        seq_t val = big_pow((seq_t)k, k);   /* k^k */
        if (val > (seq_t)n) break;
        idx_t part = (idx_t)val;            /* part <= n */
        for (size_t i = part; i <= (size_t)n; ++i) {
            dp[i] += dp[i - part];
        }
    }

    return dp[(size_t)n];
}
