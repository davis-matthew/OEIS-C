/*
 * A283529  The number of partitions of n into simple parts.
 * https://oeis.org/A283529
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into simple parts (A002110)
 * Range:     u8 n <= 63, u16 n <= 471, u32 n <= 8603, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Include common utilities if needed */
/* #include "../utils/...h"  (none required for this implementation) */

seq_t a(idx_t n)
{
    /* Compute all simple parts (A002110) <= n.
       These are 1 followed by the primorials 2, 2*3, 2*3*5, ...   */
    const idx_t primes[] = {2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,67};
    const size_t primes_cnt = sizeof(primes)/sizeof(primes[0]);

    idx_t simple_parts[20];   /* enough for all n up to very large values */
    size_t simple_count = 1;  /* 1 is always a simple part */
    simple_parts[0] = 1;

    idx_t prod = 1;
    for (size_t i = 0; i < primes_cnt; ++i) {
        if (prod > n / primes[i]) break;          /* avoid overflow */
        prod *= primes[i];
        if (prod > n) break;
        simple_parts[simple_count++] = prod;
    }

    /* Allocate dp array of size n+1 */
    seq_t *dp = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!dp) return 0;  /* allocation failed */

    for (idx_t i = 0; i <= n; ++i) dp[i] = 0;
    dp[0] = 1;  /* empty partition */

    /* Unbounded coin change DP */
    for (size_t idx = 0; idx < simple_count; ++idx) {
        idx_t part = simple_parts[idx];
        for (idx_t i = part; i <= n; ++i) {
            dp[i] += dp[i - part];
        }
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
